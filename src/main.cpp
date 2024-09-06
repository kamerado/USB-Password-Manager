#include <iostream>
#include <memory>
#include <cstdlib>
#include <cstdio>
#include <fstream>
#include <filesystem>
#include <thread>

#include "core/DatabaseManager.h"
#include "core/EncryptionUtil.h"
#include "core/MessageWorker.h"
#include "core/Logger.h"
#include "src/gui/GUI/mainwindow.h"
#include "src/gui/GUI/login.h"
#include "src/gui/GUI/setup.h"

#include <QApplication>

namespace fs = std::filesystem;

std::unique_ptr<EncryptionUtil> enc;
std::unique_ptr<NativeMessagingWorker> mw = std::make_unique<NativeMessagingWorker>();
auto future;

void createFile(const char *File) {
  std::fstream fs;
  fs.open(File, std::ios::out);
  fs.close();
}

void deleteFile(const char *File) {
  std::remove(File);
}

void handler(std::unique_ptr<EncryptionUtil>& encdec) {
  enc = std::move(encdec);
}

void startThreadHandler(bool start) {
  // start hat shiut
  if (start);
    std::cout << "started thread." << std::endl;
    future = std::async(std::launch::async, [&mw]() { mw->run(); });
}

int main(int argc, char *argv[]) {

  std::unique_ptr<Logger> logM = std::make_unique<Logger>();
  std::unique_ptr<DatabaseManager> db = std::make_unique<DatabaseManager>(logM);
  
  // auto future = std::async(std::launch::async, mw->run);
  QApplication a(argc, argv);
  MainWindow m(logM);
  QWidget::connect(&m, &MainWindow::startThread, startThreadHandler);
  int ret;
  if(!fs::exists("../../db/passwords.dbe")) {
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