#include "login.h"
#include "ui_login.h"

#include <QString>
#include <memory>
#include <src/core/EncryptionUtil.h>
#include <strings.h>

Login::Login(QWidget *parent) : QDialog(parent), ui(new Ui::Login) {
  ui->setupUi(this);
}

Login::Login(std::shared_ptr<Logger> &logM, QWidget *parent)
    : QDialog(parent), ui(new Ui::Login) {
  ui->setupUi(this);
  this->logM = std::move(logM);
}

Login::~Login() {}

void Login::getEnc() { emit sendEnc(encdec); }

void Login::setDB(std::unique_ptr<DatabaseManager> &database) {
  this->db = std::move(database);
}

void Login::on_LoginButton_clicked() {
  std::string pass = ui->UsernameInput->text().toStdString() +
                     ui->PasswordInput->text().toStdString();
  this->encdec = std::make_unique<EncryptionUtil>(pass);
  this->accept();
      // Give time for handles to be released
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

  // Decrypt db.
  this->encdec->DecryptFile();

  // TODO: If db opens properly, continue to app.
}
