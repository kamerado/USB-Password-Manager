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

template <typename T>
void startGui(T &w, MainWindow &m, std::shared_ptr<Logger> &p_logger,
              std::unique_ptr<DatabaseManager> &p_db) {
  QWidget::connect(&w, &T::sendEnc, handler);
  if (w.exec() == QDialog::Accepted) {
    w.getEnc();
    m.setEnc(enc);
    m.setDB(p_db);
    m.syncUIWithDB();
    m.show();
  }
}

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);

  std::shared_ptr<Logger> logM = std::make_shared<Logger>(Logger(true));
  std::unique_ptr<DatabaseManager> db = std::make_unique<DatabaseManager>(logM);

  MainWindow m(logM);

  int ret;
  if (!fs::exists("../db/passwords.dbe")) {
    Setup s(logM);
    startGui<Setup>(s, m, logM, db);
    ret = a.exec();
  } else {
    Login l(logM);
    startGui<Login>(l, m, logM, db);
    ret = a.exec();
  }
  std::cout << "exiting" << std::endl;
  return ret;
}
