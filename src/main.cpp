#include "core/DatabaseManager.h"
#include "core/EncryptionUtil.h"
#include "core/Logger.h"
#include "src/gui/GUI/login.h"
#include "src/gui/GUI/mainwindow.h"
#include "src/gui/GUI/setup.h"

#include <QtConcurrent/qtconcurrentrun.h>
#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <memory>

#include <QApplication>
#include <qfuture.h>
#include <qobject.h>
#include <qthread.h>

namespace fs = std::filesystem;

std::unique_ptr<EncryptionUtil> enc;

void handler(std::unique_ptr<EncryptionUtil> &encdec) {
  enc = std::move(encdec);
}

int main(int argc, char *argv[]) {

  std::shared_ptr<Logger *> logM = std::make_shared<Logger *>(new Logger(true));
  std::unique_ptr<DatabaseManager> db = std::make_unique<DatabaseManager>(logM);

  QApplication a(argc, argv);
  MainWindow m(logM);

  int ret;
  if (!fs::exists("../db/passwords.dbe")) {
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
