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

Setup::Setup(Logger* logM, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Setup)
{
    ui->setupUi(this);
    this->logM = logM;
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
    std::string pw = ui->UsernameInput->text().toStdString() + ui->PasswordInput->text().toStdString();
    this->encdec = new EncryptionUtil(pw);
    this->accept();
}

