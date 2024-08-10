#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "../../core/EncryptionUtil.h"
#include "../../core/DatabaseManager.h"
#include <src/core/Logger.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void closeEvent (QCloseEvent *event);
    void setEnc(EncryptionUtil* encddec);
    void setDB(DatabaseManager* database);

private slots:
    void on_StartButton_clicked();

    void on_AddNewButton_clicked();

    void on_exitButton_clicked();

private:
    Ui::MainWindow *ui;
    DatabaseManager* db;
    EncryptionUtil* enc;
    Logger* logM = new Logger();

};
#endif // MAINWINDOW_H
