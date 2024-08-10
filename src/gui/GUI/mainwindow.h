#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "../../core/EncryptionUtil.h"
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
    void setEnc(EncryptionUtil* encddec);
    void start();

private slots:
    void on_StartButton_clicked();

    void on_AddNewButton_clicked();

private:
    Ui::MainWindow *ui;
    DatabaseManager* dbm = new DatabaseManager();
    EncryptionUtil* enc;

};
#endif // MAINWINDOW_H
