#include "addnewwindow.h"
#include "ui_addnewwindow.h"
#include <QListWidgetItem>
#include <QWidget>
#include <iostream>

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
    if (this->pwSelected != -1) {
        auto selected = this->pwSelected;
        QString website = ui->webiteInput->text();
        QString username = ui->usernameInput->text();
        QString password = ui->passwordInput->text();
        QString space{"   "};
        QString entry = website + space + username + space + password;

        QListWidgetItem *it = ui->PasswordWidget->takeItem(this->pwSelected);
        it->setText(entry);

                // Insert the item back into the QListWidget at the same position
        ui->PasswordWidget->insertItem(selected, it);
            ui->PasswordWidget->setCurrentRow(selected);
    }
}


void addnewwindow::on_Delete_clicked()
{
    if (this->pwSelected != -1) {
    delete &current;

    ui->PasswordWidget->setCurrentRow(-1);
    }
}

void addnewwindow::on_PasswordWidget_currentRowChanged(int currentRow)
{
    this->pwSelected = currentRow;
    std::cout << "Selected QListItem: " << this->pwSelected << std::endl;
}

void addnewwindow::on_PasswordWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous) {
    this->current = *current;
}