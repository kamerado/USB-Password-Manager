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

Login::Login(Logger* logM, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
    this->logM = logM;
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
    std::string pass = ui->UsernameInput->text().toStdString() + ui->PasswordInput->text().toStdString();
    this->encdec = new EncryptionUtil(pass);
    this->accept();
    
    //TODO: Decrypt db.
    this->encdec->DecryptFile();
    

    //TODO: If db opens properly, continue to app.
}

