#include "setup.h"
#include "ui_setup.h"
#include <QString>
#include <memory>
#include <src/core/EncryptionUtil.h>
#include <string>

Setup::Setup(QWidget *parent) : QDialog(parent), ui(new Ui::Setup) {
  ui->setupUi(this);
}

Setup::Setup(std::shared_ptr<Logger> &logM, QWidget *parent)
    : QDialog(parent), ui(new Ui::Setup) {
  ui->setupUi(this);
  this->logM = std::move(logM);
  settings = std::make_unique<Settings>("../settings/settings.ini");
}

Setup::~Setup() {}

void Setup::getEnc() { emit sendEnc(encdec); }

void Setup::setDB(std::unique_ptr<DatabaseManager> &database) {
  // Setting db
  this->db = std::move(database);
}

void Setup::on_SetupButton_clicked() {
  std::string pw = ui->UsernameInput->text().toStdString() +
                   ui->PasswordInput->text().toStdString();
  this->encdec = std::make_unique<EncryptionUtil>(pw);
  this->accept();
}
