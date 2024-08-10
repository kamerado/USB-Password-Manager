#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "addnewwindow.h"
#include <QCloseEvent>
#include <QMessageBox>

#include "../../core/DatabaseManager.h"
#include "../../core/EncryptionUtil.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), 
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::MainWindow(Logger* logM, QWidget *parent)
    : QMainWindow(parent), 
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->logM = logM;
}


MainWindow::~MainWindow()
{
    delete ui;
}

// void MainWindow::start() {
//     dbm->testFunctionality();
// }

void MainWindow::closeEvent (QCloseEvent *event)
{
    this->enc->EncryptFile();
    event->accept();
}

void MainWindow::setEnc(EncryptionUtil* encdec) {
    this->enc = encdec;
}

void MainWindow::setDB(DatabaseManager* database) {
    this->db = database;
}

void MainWindow::on_StartButton_clicked()
{
    db->testFunctionality();
}


void MainWindow::on_AddNewButton_clicked()
{
    addnewwindow addnew;
    addnew.setModal(true);
    addnew.exec();
}

void MainWindow::on_exitButton_clicked()
{
    this->close();
}
