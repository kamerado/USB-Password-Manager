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
    QString pw = ui->UsernameInput->text() + ui->PasswordInput->text();

    //TODO: Decrypt db.
    

    //TODO: If db opens properly, continue to app.
}

