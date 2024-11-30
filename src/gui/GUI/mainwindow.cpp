#include "src/gui/GUI/mainwindow.h"
#include "./ui_mainwindow.h"
#include "src/core/Logger.h"
#include "src/gui/GUI/settingsdialog.h"
#include <QCloseEvent>
#include <QFuture>
#include <QItemSelectionModel>
#include <QList>
#include <QMessageBox>
#include <QtConcurrent/QtConcurrent>
#include <QtConcurrent/qtconcurrentrun.h>
#include <QtCore/qmetatype.h>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <memory>
#include <nlohmann/json.hpp>
#include <qapplication.h>
#include <qfuture.h>
#include <qglobal.h>
#include <qmessagebox.h>
#include <qnamespace.h>
#include <qobject.h>
#include <qobjectdefs.h>
#include <qpushbutton.h>
#include <qwebsocket.h>
#include <vector>

#include "src/core/WebSocket.h"

#include "../../core/DatabaseManager.h"
#include "../../core/EncryptionUtil.h"

class DomainValidator {
private:
  // Common generic TLDs
  const std::vector<std::string> genericTLDs = {
      ".com",  ".org",  ".net",  ".edu", ".gov", ".mil",  ".info",
      ".biz",  ".name", ".mobi", ".pro", ".int", ".coop", ".jobs",
      ".tech", ".app",  ".io",   ".co",  ".ai",  ".dev",  ".cloud"};

  std::vector<std::string> split(const std::string &str, char delimiter) const {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(str);

    while (std::getline(tokenStream, token, delimiter)) {
      if (!token.empty()) {
        tokens.push_back(token);
      }
    }
    return tokens;
  }

  bool isValidLabel(const std::string &label) const {
    if (label.empty() || label.length() > 63)
      return false;

    // Check first and last characters
    if (!std::isalnum(label.front()) || !std::isalnum(label.back()))
      return false;

    // Check for valid characters and no consecutive hyphens
    for (size_t i = 0; i < label.length(); ++i) {
      if (!std::isalnum(label[i]) && label[i] != '-')
        return false;
      if (label[i] == '-' && i + 1 < label.length() && label[i + 1] == '-')
        return false;
    }

    return true;
  }

  std::string toLower(std::string str) const {
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);
    return str;
  }

public:
  bool isValidDomain(std::string domain) const {
    try {
      // Convert to lowercase
      domain = toLower(domain);

      // Split into labels
      auto labels = split(domain, '.');

      // Domain must have at least 2 parts
      if (labels.size() < 2)
        return false;

      // Validate each label
      for (const auto &label : labels) {
        if (!isValidLabel(label))
          return false;
      }

      // Check TLD
      std::string tld = "." + labels.back();
      for (const auto &validTLD : genericTLDs) {
        if (tld == validTLD)
          return true;
      }

      return false;

    } catch (const std::exception &e) {
      std::cerr << "Validation error: " << e.what() << std::endl;
      return false;
    }
  }
};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
}

MainWindow::MainWindow(std::shared_ptr<Logger> &logM, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), logger(logM) {
  // logger->log(DEBUG, "DEBUG: MainWindow constructor starting");
  LOG_DEBUG(logM, "MainWindow constructor starting.");
  ui->setupUi(this);
  // startbtn = dynamic_cast<QPushButton *>(ui->StartButton);
  ui->CTable->setColumnCount(4);
  settings = std::make_shared<Settings>("../settings/settings.ini");
  QStringList headers;
  headers << "Website"
          << "Email"
          << "Username"
          << "Password";
  ui->CTable->setHorizontalHeaderLabels(headers);
  ui->CTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
  ui->CTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
  ui->CTable->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
  ui->CTable->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);
  if (server == nullptr) {
    server = std::make_unique<WebSocketServer>(logger);
    // Handle incoming messages on the main thread using queued connections
    QObject::connect(&(*server), &WebSocketServer::messageReceived, this,
                     &MainWindow::parseMessage, Qt::QueuedConnection);

    LOG_DEBUG(logger, "MainWindow constructor completed");
  }
}

MainWindow::~MainWindow() {
  stopServer();
  if (db) {
    db->close();
  }
  // waitForServerStop();
  delete ui;
}

// void MainWindow::startServer() {
//   if (server == nullptr) {
//     server = std::make_unique<WebSocketServer>(logger);
//
//     // QObject::connect(serverThread, &QThread::started, server,
//     //                  [this]() { server->start(8080); });
//     // QObject::connect(server, &WebSocketServer::messageReceived, this,
//     //                  &MainWindow::parseMessage, Qt::QueuedConnection);
//     // QObject::connect(serverThread, &QThread::finished, server,
//     //                  &QObject::deleteLater);
//   }
// }

void MainWindow::stopServer() {
  if (server != nullptr) {
    server->stop();
  }
}

// void MainWindow::waitForServerStop() {
//   if (!serverThread->isRunning()) {
//     serverThread->wait();
//   }
//   server = nullptr;
// }

void MainWindow::on_StartButton_toggled(bool checked) {
  LOG_DEBUG(logger, "on_StartButton_toggled called with checked = {}",
            (checked) ? "true" : "false");
  nlohmann::json message = {{"action", "toggle-background"}, {"isOn", checked}};

  std::string jsonString = message.dump();
  server->sendMessage(jsonString);
}

void MainWindow::receiveToggleSignal(bool &status) {
  LOG_DEBUG(logger, "receiveToggleSignal entry with status = {}",
            (status) ? "true" : "false");

  if (!ui) {
    LOG_ERROR(logger, "'ui' is not initialized.");
    return;
  }

  if (!ui->StartButton) {
    LOG_ERROR(logger, "'StartButton' is not initialized in the UI.");
    return;
  }
  bool tmp = status;
  if (ui->StartButton) {
    ui->StartButton->setChecked(tmp);
  } else {
    LOG_ERROR(logger, "startbtn is not a QPushButton.");
  }
}

void MainWindow::parseMessage(const QString &message) {
  LOG_DEBUG(logger, "parseMessage received: {}", message.toStdString());
  try {
    using json = nlohmann::json;
    json j = json::parse(message.toStdString());

    if (!j.contains("type") || !j["type"].is_string()) {
      LOG_ERROR(logger, "Missing or invalid 'type' field in message.");
      return;
    }

    const std::string &typeStr = j["type"];

    if (typeStr == "status") {
      if (!j.contains("status") || !j["status"].is_boolean()) {
        LOG_ERROR(logger, "'status' field missing or not a boolean.");
        return;
      }

      bool status = j["status"];
      LOG_DEBUG(logger, "Status value extracted: {}",
                (status) ? "true" : "false");

      bool statusCopy = status;
      receiveToggleSignal(statusCopy);
    }
    if (typeStr == "check-entry") {
      if (!j.contains("website") || !j["website"].is_string()) {
        LOG_ERROR(logger, "'website' field missing or not a string.");
        return;
      }
      QString tmp = QString::fromStdString(std::string(j["website"]));
      std::vector<QString> result = db->executeCheck(tmp);
      if (result.size() > 0) {
        json entry = {{"username", result.at(0).toStdString()},
                      {"password", result.at(1).toStdString()}};
        json message = {{"action", "receive-entry"}, {"entry", entry}};

        std::string jsonString = message.dump();
        LOG_DEBUG(logger, jsonString.c_str());
        server->sendMessage(jsonString);
        return;
      } else {
        json message = {{"action", "receive-entry"},
                        {"website", std::string(j["website"])}};

        std::string jsonString = message.dump();
        LOG_INFO(logger, jsonString.c_str());
        server->sendMessage(jsonString);

        LOG_DEBUG(logger, "Nothing in vector.");
        return;
      }
    }
    if (typeStr == "init") {
      LOG_DEBUG(logger, "Received init request.");
      json message = {{"isOn", ui->StartButton->isChecked()},
                      {"action", "init-response"}};
      std::string jsonstring = message.dump();
      server->sendMessage(jsonstring);
      LOG_DEBUG(logger, "Sent init message.");
    }
    if (typeStr == "new-entry") {
      if (!j.contains("website") || !j["website"].is_string()) {
        LOG_ERROR(logger, "'website' field missing or not a string.");
        return;
      }
      if (!j.contains("username") || !j["username"].is_string()) {
        LOG_ERROR(logger, "'username' field missing or not a string.");
        return;
      }
      if (!j.contains("password") || !j["password"].is_string()) {
        LOG_ERROR(logger, "'password' field missing or not a string.");
        return;
      }
      QString website = QString::fromStdString(j["website"]);
      QString username = QString::fromStdString(j["username"]);
      QString password = QString::fromStdString(j["password"]);
      addNewEntry(website, settings->getMasterEmail(), username, password);
      LOG_DEBUG(logger, "Inserted new entry.");
    }
  } catch (const nlohmann::json::parse_error &e) {
    LOG_ERROR(logger, "Parsing JSON Error: {}", e.what());
  } catch (const std::exception &e) {
    LOG_ERROR(logger, "Unknown Error: {}", e.what());
  }
}

void MainWindow::closeEvent(QCloseEvent *event) {
  // First stop the server if running
  if (server != nullptr) {
    stopServer();
  }

  // Close database connection
  if (db != nullptr) {
    db->close(); // Make sure DatabaseManager has a close() method
  }

  // Give time for handles to be released
  std::this_thread::sleep_for(std::chrono::milliseconds(100));

  // Now encrypt the file
  if (enc != nullptr) {
    this->enc->EncryptFile();
  }

  event->accept();
}

void MainWindow::setEnc(std::unique_ptr<EncryptionUtil> &encdec) {
  this->enc = std::move(encdec);
}

void MainWindow::setDB(std::unique_ptr<DatabaseManager> &database) {
  this->db = std::move(database);
}

void MainWindow::on_exitButton_clicked() { this->close(); }

void MainWindow::addNewEntry(QString website, QString email, QString username,
                             QString password) {
  if (!isValidDomain(website.toStdString())) {
    QMessageBox::warning(this, "Invalid Website",
                         "The website must end with a valid domain name.");
    return;
  }
  if (username == "") {
    username = settings->getDefaultUsername();
  }
  this->numRows++;
  if (this->db->addEntry(numRows, website, email, username, password)) {
    ui->CTable->setRowCount(numRows);
    ui->CTable->setItem(numRows - 1, 0, new QTableWidgetItem(website));
    ui->CTable->setItem(numRows - 1, 1, new QTableWidgetItem(email));
    ui->CTable->setItem(numRows - 1, 2, new QTableWidgetItem(username));
    ui->CTable->setItem(numRows - 1, 3, new QTableWidgetItem(password));
  }
}

void MainWindow::on_Add_clicked() {
  QString website = ui->webiteInput->text();
  QString email = settings->getMasterEmail();
  QString username = ui->usernameInput->text();
  QString password = ui->passwordInput->text();

  addNewEntry(website, email, username, password);
}

void MainWindow::on_Edit_clicked() {
  QString website = ui->webiteInput->text();
  QString email = settings->getMasterEmail();
  QString username = ui->usernameInput->text();
  QString password = ui->passwordInput->text();

  int row = getCurrRow();
  std::cout << row << std::endl;

  if (row >= 0) {
    if (!isValidDomain(website.toStdString())) {
      QMessageBox::warning(this, "Invalid Website",
                           "The website must end with a valid domain name.");
      return;
    }

    int id = row + 1;
    std::cout << "Editing row ID: " << id << std::endl;
    if (this->db->updateEntry(id, website, email, username, password)) {
      ui->CTable->setItem(row, 0, new QTableWidgetItem(website));
      ui->CTable->setItem(row, 1, new QTableWidgetItem(email));
      ui->CTable->setItem(row, 2, new QTableWidgetItem(username));
      ui->CTable->setItem(row, 3, new QTableWidgetItem(password));
    } else {
      QMessageBox::warning(this, "Update Failed",
                           "Failed to update the entry in the database.");
    }
  } else {
    QMessageBox::warning(this, "Selection Error",
                         "No row is selected for editing.");
  }
}

void MainWindow::on_Delete_clicked() {
  int row = getCurrRow();

  int id = row + 1;

  LOG_DEBUG(logger, "Row:{}", row);
  if (row >= 0) {
    if (this->db->deleteEntry(id)) {
      ui->CTable->removeRow(row);
      this->numRows--;

      ui->CTable->clearSelection();
    } else {
      QMessageBox::warning(this, "Deletion Failed",
                           "Failed to delete the entry from the database");
    }
  } else {
    QMessageBox::warning(this, "Selection Error",
                         "No row is selected for deletion.");
  }
}

void MainWindow::on_DeleteAll_clicked() {

  QMessageBox::StandardButton reply;
  reply = QMessageBox::question(this, "Delete All",
                                "Are you sure you want to delete all entrys?",
                                QMessageBox::Yes | QMessageBox::No);
  if (reply == QMessageBox::Yes) {
    ui->CTable->setRowCount(0);
    this->numRows = 0;

    if (this->db->deleteAllEntries()) {
      QMessageBox::information(this, "Deleted",
                               "All entries have been deleted.");
    } else {
      QMessageBox::warning(this, "Error",
                           "Failed to delete entries from the database.");
    }
  }
}

void MainWindow::on_SettingsButton_clicked() {
  SettingsDialog settingsDialog(logger, settings, this);
  settingsDialog.exec();
}

int MainWindow::getCurrRow() {
  QModelIndexList selectedRows = ui->CTable->selectionModel()->selectedRows();
  if (!selectedRows.isEmpty()) {
    return selectedRows.at(0).row();
  } else {
    return -1;
  }
}

// TODO: implement correct domain checking
bool MainWindow::isValidDomain(const std::string &website) {
  std::string domain;
  auto idx = website.find(".");
  if (idx != std::string::npos) {
    domain = website.substr(idx, website.length());
  }
  DomainValidator validator;
  return validator.isValidDomain(domain);
}

QTableWidgetItem* MainWindow::hidePassword(QTableWidgetItem *itm) {
  itm->setText("********");
  itm->setData(Qt::AccessibleTextRole, false);

  return itm;
}

QTableWidgetItem* MainWindow::showPassword(QTableWidgetItem *itm) {
  itm->setText(itm->data(Qt::AccessibleTextRole).toString());
  itm->setData(Qt::AccessibleTextRole, true);

  return itm;
}

void MainWindow::syncUIWithDB() {
  QList<rowEntry> results = this->db->queryAll();
  for (const rowEntry &row : results) {
    QString website = row.website;
    QString email = row.email;
    QString username = row.username;
    QString password = row.password;

    this->numRows++;

    ui->CTable->setRowCount(numRows);
    ui->CTable->setItem(numRows - 1, 0, new QTableWidgetItem(website));
    ui->CTable->setItem(numRows - 1, 1, new QTableWidgetItem(email));
    ui->CTable->setItem(numRows - 1, 2, new QTableWidgetItem(username));
    ui->CTable->setItem(numRows - 1, 3, hidePassword(new QTableWidgetItem(password)));

  }
}
