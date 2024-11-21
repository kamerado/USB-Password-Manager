#include "src/gui/GUI/mainwindow.h"
#include "./ui_mainwindow.h"
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
// #include <sys/socket.h>
#include <vector>

#include "src/core/WebSocket.h"

#include "../../core/DatabaseManager.h"
#include "../../core/EncryptionUtil.h"

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
  ui->CTable->setColumnCount(3);
  QStringList headers;
  headers << "Website"
          << "Username"
          << "Password";
  ui->CTable->setHorizontalHeaderLabels(headers);
  ui->CTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
  ui->CTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
  ui->CTable->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
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

void MainWindow::startServer() {
  if (server == nullptr) {
    server = std::make_unique<WebSocketServer>(logger);

    // QObject::connect(serverThread, &QThread::started, server,
    //                  [this]() { server->start(8080); });
    // QObject::connect(server, &WebSocketServer::messageReceived, this,
    //                  &MainWindow::parseMessage, Qt::QueuedConnection);
    // QObject::connect(serverThread, &QThread::finished, server,
    //                  &QObject::deleteLater);
  }
}

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
  if (checked) {
    // TODO: startService();
  } else {
    // TODO: stopService();
  }
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

void MainWindow::parseMessage(const QString &message,
                              const QWebSocket *client) {
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
        server->sendMessage(client, jsonString);
        return;
      } else {
        json message = {{"action", "receive-entry"},
                        {"website", std::string(j["website"])}};

        std::string jsonString = message.dump();
        LOG_INFO(logger, jsonString.c_str());
        server->sendMessage(client, jsonString);

        LOG_DEBUG(logger, "Nothing in vector.");
        return;
      }
    }
    if (typeStr == "get-default-username") {
      // TODO: Handle get-default-username request.
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

void MainWindow::on_Add_clicked() {
  QString website = ui->webiteInput->text();
  QString username = ui->usernameInput->text();
  QString password = ui->passwordInput->text();

  if (!isValidDomain(website.toStdString())) {
    QMessageBox::warning(this, "Invalid Website",
                         "The website must end with a valid domain name.");
    return;
  }

  this->numRows++;
  if (this->db->addEntry(numRows, website, username, password)) {
    ui->CTable->setRowCount(numRows);
    ui->CTable->setItem(numRows - 1, 0, new QTableWidgetItem(website));
    ui->CTable->setItem(numRows - 1, 1, new QTableWidgetItem(username));
    ui->CTable->setItem(numRows - 1, 2, new QTableWidgetItem(password));
  }
}

void MainWindow::on_Edit_clicked() {
  QString website = ui->webiteInput->text();
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
    if (this->db->updateEntry(id, website, username, password)) {
      ui->CTable->setItem(row, 0, new QTableWidgetItem(website));
      ui->CTable->setItem(row, 1, new QTableWidgetItem(username));
      ui->CTable->setItem(row, 2, new QTableWidgetItem(password));
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

  if (row >= 0) {
    int id = ui->CTable->item(row, 0)->text().toInt();
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
  SettingsDialog settingsDialog(logger, this);
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

  if (domain == ".com" || domain == ".org" || domain == ".net") {
    return true;
  } else {
    return false;
  }
}

void MainWindow::syncUIWithDB() {
  QList<rowEntry> results = this->db->queryAll();
  for (const rowEntry &row : results) {
    QString website = row.website;
    QString username = row.username;
    QString password = row.password;

    this->numRows++;

    ui->CTable->setRowCount(numRows);
    ui->CTable->setItem(numRows - 1, 0, new QTableWidgetItem(website));
    ui->CTable->setItem(numRows - 1, 1, new QTableWidgetItem(username));
    ui->CTable->setItem(numRows - 1, 2, new QTableWidgetItem(password));
  }
}
