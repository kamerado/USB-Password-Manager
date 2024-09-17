#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QCloseEvent>
#include <QItemSelectionModel>
#include <QList>
#include <QMessageBox>
#include <iostream>
#include <memory>

#include "../../core/DatabaseManager.h"
#include "../../core/EncryptionUtil.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  startNativeMessagingThread();
}

MainWindow::MainWindow(std::unique_ptr<Logger> &logM, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  this->logM = std::move(logM);
  ui->CTable->setColumnCount(3);
  QStringList headers;
  headers << "Website" << "Username" << "Password";
  ui->CTable->setHorizontalHeaderLabels(headers);
  ui->CTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
  ui->CTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
  ui->CTable->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
}

MainWindow::~MainWindow() {
  delete ui;
  if (workerThread) {
    workerThread->quit();
    workerThread->wait();
  }
}

void MainWindow::closeEvent(QCloseEvent *event) {
  // this->db->~DatabaseManager();
  this->enc->EncryptFile();
  // this->enc->~EncryptionUtil();
  event->accept();
}

void MainWindow::setEnc(std::unique_ptr<EncryptionUtil> &encdec) {
  this->enc = std::move(encdec);
}

void MainWindow::setDB(std::unique_ptr<DatabaseManager> &database) {
  this->db = std::move(database);
}

void MainWindow::on_StartButton_toggled(bool checked) {
  // this->workerThread->run();
  std::cout << "mainwindow.cpp: starting native messaging thread." << std::endl;
  if (checked) {
    // startNativeMessagingThread();
    std::cout << "Clicked." << std::endl;
    this->startMessageThread();
  } else {
    std::cout << "Unclicked." << std::endl;
    this->endMessageThread();
  }
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

void MainWindow::startMessageThread() { emit startThread(true); }
void MainWindow::endMessageThread() { emit startThread(false); }

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
  // Add tegex domain check.
  // std::regex
  // domainPattern(R"(^(?:(?!-)[A-Za-z0-9-]{1,63}(?<!-)\.)+(com|net|org)$)");
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
  // return std::regex_match(website, domainPattern);
}

void MainWindow::syncUIWithDB() {
  QList<rowEntry> results = this->db->queryAll();
  for (const rowEntry &row : results) {
    // int id = row.id; unused.
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

// REDO
void MainWindow::startNativeMessagingThread() {
  // workerThread = new QThread(this);
  // this->worker = new NativeMessagingWorker();

  // // Move the worker to the thread
  // worker->moveToThread(workerThread);

  // // Connect signals and slots
  // connect(workerThread, &QThread::started, worker,
  // &NativeMessagingWorker::run); connect(worker,
  // &NativeMessagingWorker::messageReceived, this,
  // &MainWindow::onMessageReceived);

  // // Start the thread
  // workerThread->start();
}

void MainWindow::onMessageReceived(const QString &message) {
  QMessageBox::information(this, "Message from native app:", message);
  // Update the GUI with the received message
}
