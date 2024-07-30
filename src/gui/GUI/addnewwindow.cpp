#include "addnewwindow.h"
#include "ui_addnewwindow.h"
#include <QListWidgetItem>
#include <QWidget>

addnewwindow::addnewwindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addnewwindow)
{
    ui->setupUi(this);
}

addnewwindow::~addnewwindow()
{
    delete ui;
}

void addnewwindow::on_Add_clicked()
{
    QString website = ui->webiteInput->text();
    QString username = ui->usernameInput->text();
    QString password = ui->passwordInput->text();
    QString space{"   "};
    QString entry = website + space + username + space + password;

    ui->PasswordWidget->addItem(entry);
}


void addnewwindow::on_Edit_clicked()
{
    QString website = ui->webiteInput->text();
    QString username = ui->usernameInput->text();
    QString password = ui->passwordInput->text();
    QString space{"   "};
    QString entry = website + space + username + space + password;

    QListWidgetItem *it = ui->PasswordWidget->takeItem(pwSelected);
    it->setText(entry);
}


void addnewwindow::on_Delete_clicked()
{
    QListWidgetItem *it = ui->PasswordWidget->takeItem(pwSelected);
    delete it;

    ui->PasswordWidget->setCurrentRow(-1);
}

void addnewwindow::on_PasswordWidget_currentRowChanged(int currentRow)
{
    pwSelected = currentRow;
}

void addnewwindow::on_PasswordWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous) {

}