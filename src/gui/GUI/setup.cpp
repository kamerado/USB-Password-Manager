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
  if (ui->UsernameInput->text().isEmpty() || ui->PasswordInput->text().isEmpty()) {
    QMessageBox::warning(this, "Invalid Input", "Please enter a valid username and password.");
    return;
  }
  if (ui->PasswordInput->text() != ui->ConfirmPassword->text()) {
    QMessageBox::warning(this, "Invalid Input", "Passwords do not match.");
    return;
  }
  if (ui->PasswordInput->text().length() < 12) {
    QMessageBox::warning(this, "Invalid Input", "Password must be at least 12 characters long.");
    return;
  }
  std::string username = ui->UsernameInput->text().toStdString();
  std::string pw = ui->PasswordInput->text().toStdString();
  settings->setDefaultUsername(QString::fromStdString(username));
  this->encdec = std::make_unique<EncryptionUtil>(pw);
  this->accept();
}
