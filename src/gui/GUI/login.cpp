#include "login.h"
#include "ui_login.h"

#include <strings.h>
#include <iostream>
#include <QString>
#include <src/core/EncryptionUtil.h>

Login::Login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
}

Login::~Login()
{
    delete ui;
}

void Login::getEnc() {
     emit sendEnc(encdec);
}

void Login::setDB(DatabaseManager* database) {
    this->db = database;
}

void Login::on_LoginButton_clicked()
{
    const char* pass = (ui->UsernameInput->text().toStdString() + ui->PasswordInput->text().toStdString()).c_str();
    std::cout << "password: " << (ui->UsernameInput->text().toStdString() + ui->PasswordInput->text().toStdString()).c_str() << std::endl;
    this->encdec = new EncryptionUtil(pass);
    this->accept();
    
    //TODO: Decrypt db.
    this->encdec->DecryptFile();
    

    //TODO: If db opens properly, continue to app.
}

