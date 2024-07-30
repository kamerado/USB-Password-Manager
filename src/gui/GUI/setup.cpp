#include "setup.h"
#include "ui_setup.h"
#include <string>

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
    std::string pw = ui->UsernameInput->toString() + ui->PasswordInput->toString();
    

}

