#include "setup.h"
#include "ui_setup.h"
#include <string>
#include <QString>
#include <iostream>
#include <memory>
#include <src/core/EncryptionUtil.h>

Setup::Setup(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Setup)
{
    ui->setupUi(this);
}

Setup::Setup(std::unique_ptr<Logger>& logM, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Setup)
{
    ui->setupUi(this);
    this->logM = std::move(logM);
}

Setup::~Setup(){}

void Setup::getEnc() {
     emit sendEnc(encdec);
}

void Setup::setDB(std::unique_ptr<DatabaseManager>& database) {
    // Setting db
    this->db = std::move(database);
}

void Setup::on_SetupButton_clicked()
{
    std::string pw = ui->UsernameInput->text().toStdString() + ui->PasswordInput->text().toStdString();
    this->encdec = std::make_unique<EncryptionUtil>(pw);
    this->accept();
}

