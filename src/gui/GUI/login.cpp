#include "login.h"
#include "ui_login.h"

#include <strings.h>

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
    std::string pw = ui->UsernameInput->toString() + ui->PasswordInput->toString();

    //TODO: Decrypt db.
    

    //TODO: If db opens properly, continue to app.
}

