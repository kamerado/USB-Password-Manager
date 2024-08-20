#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "../../core/EncryptionUtil.h"
#include "../../core/DatabaseManager.h"
#include <src/core/Logger.h>
#include <memory>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    MainWindow(std::unique_ptr<Logger>& logM, QWidget *parent = nullptr);
    ~MainWindow();
    void closeEvent (QCloseEvent *event);
    void setEnc(std::unique_ptr<EncryptionUtil>& encddec);
    void setDB(std::unique_ptr<DatabaseManager>& database);
    void syncUIWithDB();
    

private slots:
    void on_StartButton_clicked();

    void on_exitButton_clicked();

    void on_Add_clicked();

    void on_Edit_clicked();

    void on_Delete_clicked();

private:
    Ui::MainWindow *ui;
    int numRows = 0;
    std::unique_ptr<DatabaseManager> db;
    std::unique_ptr<EncryptionUtil> enc;
    std::unique_ptr<Logger> logM;

    int getCurrRow();
};
#endif // MAINWINDOW_H
