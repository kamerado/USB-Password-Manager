#include "core/DatabaseManager.h"
#include "core/EncryptionUtil.h"
#include "core/Logger.h"
#include "core/MessageWorker.h"
#include "src/gui/GUI/login.h"
#include "src/gui/GUI/mainwindow.h"
#include "src/gui/GUI/setup.h"

#include <QtConcurrent/qtconcurrentrun.h>
#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>

#include <QApplication>
#include <qfuture.h>
#include <qthread.h>

namespace fs = std::filesystem;

std::unique_ptr<EncryptionUtil> enc;
std::unique_ptr<NativeMessagingWorker> mw =
    std::make_unique<NativeMessagingWorker>();
QFuture<void> future;

void createFile(const char *File) {
  std::fstream fs;
  fs.open(File, std::ios::out);
  fs.close();
}

void deleteFile(const char *File) { std::remove(File); }

void handler(std::unique_ptr<EncryptionUtil> &encdec) {
  enc = std::move(encdec);
}

void startThreadHandler(bool start) {
  // Handle thread
  if (start) {
    future = QtConcurrent::run([] { mw->run(); });
  } else {
    future = QFuture<void>();
  }
}

int main(int argc, char *argv[]) {

  std::unique_ptr<Logger> logM = std::make_unique<Logger>();
  std::unique_ptr<DatabaseManager> db = std::make_unique<DatabaseManager>(logM);

  // auto future = std::async(std::launch::async, mw->run);
  QApplication a(argc, argv);
  MainWindow m(logM);
  QWidget::connect(&m, &MainWindow::startThread, startThreadHandler);
  int ret;
  if (!fs::exists("../../db/passwords.dbe")) {
    Setup s(logM);
    QWidget::connect(&s, &Setup::sendEnc, handler);
    if (s.exec() == QDialog::Accepted) {
      s.getEnc();
      m.setEnc(enc);
      m.setDB(db);
      m.syncUIWithDB();
      m.show();
    }
    ret = a.exec();
  } else {
    Login l(logM);
    QWidget::connect(&l, &Login::sendEnc, handler);
    if (l.exec() == QDialog::Accepted) {
      l.getEnc();
      m.setEnc(enc);
      m.setDB(db);
      m.syncUIWithDB();
      m.show();
    }
    ret = a.exec();
  }
  std::cout << "exiting" << std::endl;
  return ret;
}
