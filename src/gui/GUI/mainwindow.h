#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "../../core/DatabaseManager.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void start();

private:
    Ui::MainWindow *ui;
    DatabaseManager* dbm = new DatabaseManager("../../../db/passwords.db");

signals:
    void start();
};
#endif // MAINWINDOW_H
