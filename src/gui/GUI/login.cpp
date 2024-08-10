#include "login.h"
#include "ui_login.h"

#include <strings.h>
#include <QString>

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

void Login::on_LoginButton_clicked()
{
    const char* pw = (ui->UsernameInput->text().toStdString() + ui->PasswordInput->text().toStdString()).c_str();
    encdec = new EncryptionUtil(pw);
    this->accept();
    this->~Login();

    //TODO: Decrypt db.
    encdec->DecryptFile();

    //TODO: If db opens properly, continue to app.
}

