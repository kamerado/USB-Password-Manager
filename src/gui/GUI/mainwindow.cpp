#include "../../../src/gui/GUI/mainwindow.h"
#include "./ui_mainwindow.h"
#include <QCloseEvent>
#include <QFuture>
#include <QItemSelectionModel>
#include <QList>
#include <QMessageBox>
#include <QtConcurrent/QtConcurrent>
#include <QtConcurrent/qtconcurrentrun.h>
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
#include <qt5/QtCore/qchar.h>
#include <qt5/QtCore/qthread.h>
#include <sys/socket.h>

#include "../src/core/WebSocket.h"

#include "../../core/DatabaseManager.h"
#include "../../core/EncryptionUtil.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
}

MainWindow::MainWindow(std::unique_ptr<Logger> &logM, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  this->logM = std::move(logM);
  // startbtn = dynamic_cast<QPushButton *>(ui->StartButton);
  ui->CTable->setColumnCount(3);
  QStringList headers;
  headers << "Website" << "Username" << "Password";
  ui->CTable->setHorizontalHeaderLabels(headers);
  ui->CTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
  ui->CTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
  ui->CTable->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
  if (server == nullptr) {
    server = new WebSocketServer(this);
    serverThread = new QThread(this);
    server->moveToThread(serverThread);

    QObject::connect(serverThread, &QThread::started, server,
                     [this]() { server->start(8080); });
    // QObject::connect(server, &WebSocketServer::messageReceived, this,
    //                  &MainWindow::parseMessage);
    QObject::connect(serverThread, &QThread::finished, server,
                     &QObject::deleteLater);

    // Handle incoming messages on the main thread using queued connections
    QObject::connect(server, &WebSocketServer::messageReceived, this,
                     &MainWindow::parseMessage, Qt::QueuedConnection);

    std::cout << "DEBUG: MainWindow constructor starting" << std::endl;

    // Ensure UI is initialized before setting up connections
    // QMetaObject::invokeMethod(
    //     this, [this]() { setupConnections(); }, Qt::QueuedConnection);
    serverThread->start();
    std::cout << "DEBUG: MainWindow constructor completed" << std::endl;
  }
}

MainWindow::~MainWindow() {
  stopServer();
  waitForServerStop();
  delete ui;
}

void MainWindow::startServer() {
  if (server == nullptr) {
    server = new WebSocketServer(this);
    serverThread = new QThread(this);
    server->moveToThread(serverThread);

    QObject::connect(serverThread, &QThread::started, server,
                     [this]() { server->start(8080); });
    QObject::connect(server, &WebSocketServer::messageReceived, this,
                     &MainWindow::parseMessage);
    QObject::connect(serverThread, &QThread::finished, server,
                     &QObject::deleteLater);
    serverThread->start();
  }
}

void MainWindow::stopServer() {
  if (server) {
    server->stop();
  }
  serverThread->quit();
  serverThread->wait();
}

void MainWindow::waitForServerStop() {
  if (!serverThread->isRunning()) {
    serverThread->wait();
  }
  delete server;
  server = nullptr;
}

void MainWindow::on_StartButton_toggled(bool checked) {
  std::cout << "DEBUG: on_StartButton_toggled called with checked = " << checked
            << std::endl;
  if (checked) {
    // startService();
  } else {
    // stopService();
  }
}

void MainWindow::receiveToggleSignal(bool &status) {
  std::cout << "DEBUG: receiveToggleSignal entry with status = " << status
            << std::endl;

  if (!ui) {
    std::cerr << "Error: 'ui' is not initialized." << std::endl;
    return;
  }

  if (!ui->StartButton) {
    std::cerr << "Error: 'StartButton' is not initialized in the UI."
              << std::endl;
    return;
  }
  bool tmp = status;
  if (ui->StartButton) {
    ui->StartButton->setChecked(tmp);
  } else {
    std::cerr << "Error: startbtn is not a QPushButton." << std::endl;
  }
}

void MainWindow::parseMessage(const QString &message) {
  std::cout << "DEBUG: parseMessage received: " << message.toStdString()
            << std::endl;
  try {
    using json = nlohmann::json;
    json j = json::parse(message.toStdString());

    if (!j.contains("type") || !j["type"].is_string()) {
      std::cerr << "Error: Missing or invalid 'type' field in message."
                << std::endl;
      return;
    }

    const std::string &typeStr = j["type"];

    if (typeStr == "status") {
      if (!j.contains("status") || !j["status"].is_boolean()) {
        std::cerr << "Error: 'status' field missing or not a boolean."
                  << std::endl;
        return;
      }

      bool status = j["status"];
      std::cout << "DEBUG: Status value extracted: " << status << std::endl;

      bool statusCopy = status;
      receiveToggleSignal(statusCopy);
    }
  } catch (const nlohmann::json::parse_error &e) {
    std::cerr << "Error parsing JSON: " << e.what() << std::endl;
  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }
}
void MainWindow::setupConnections() {
  // std::cout << "DEBUG: Setting up connections" << std::endl;
  //
  // if (!ui || !ui->StartButton) {
  //   std::cerr << "Error: UI components not properly initialized" <<
  //   std::endl; return;
  // }
  //
  // // Configure button
  // ui->StartButton->setCheckable(true);
  //
  // // Connect using new-style signal/slot syntax
  // connect(ui->StartButton, &QPushButton::toggled, this,
  //         &MainWindow::on_StartButton_toggled,
  //         Qt::QueuedConnection); // Use QueuedConnection for thread safety
  //
  // std::cout << "DEBUG: Connections established" << std::endl;
}

void MainWindow::closeEvent(QCloseEvent *event) {
  this->enc->EncryptFile();
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
