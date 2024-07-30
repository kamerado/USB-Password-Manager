#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "addnewwindow.h"

#include "../../core/DatabaseManager.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// void MainWindow::start() {
//     dbm->testFunctionality();
// }

void MainWindow::on_StartButton_clicked()
{
    dbm->testFunctionality();
}


void MainWindow::on_AddNewButton_clicked()
{
    addnewwindow addnew;
    addnew.setModal(true);
    addnew.exec();
}

