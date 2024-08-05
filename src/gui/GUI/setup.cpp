#include "setup.h"
#include "ui_setup.h"
#include <string>
#include <QString>
#include <iostream>


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

void Setup::on_SetupButton_clicked()
{
    QString pw = ui->UsernameInput->text() + ui->PasswordInput->text();
    std::cout << pw.toStdString() << std::endl;
    dbm

    

}

