#include "setup.h"
#include "ui_setup.h"
#include <string>
#include <QString>
#include <iostream>
#include <src/core/EncryptionUtil.h>

Setup::Setup(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Setup)
{
    ui->setupUi(this);
}

Setup::~Setup()
{
    delete ui;
}

void Setup::getEnc() {
     emit sendEnc(encdec);
}

void Setup::setDB(DatabaseManager* database) {
    // Setting db
    this->db = database;
}

void Setup::on_SetupButton_clicked()
{
    const char* pw = (ui->UsernameInput->text().toStdString() + ui->PasswordInput->text().toStdString()).c_str();
    std::cout << "password: " << pw << std::endl;
    this->encdec = new EncryptionUtil(pw);
    this->accept();
}

