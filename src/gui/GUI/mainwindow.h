#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "../../core/DatabaseManager.h"
#include "../../core/EncryptionUtil.h"
#include "../../core/MessageWorker.h"
#include <memory>
#include <src/core/Logger.h>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  MainWindow(std::unique_ptr<Logger> &logM, QWidget *parent = nullptr);
  ~MainWindow();
  void closeEvent(QCloseEvent *event);
  void setEnc(std::unique_ptr<EncryptionUtil> &encddec);
  void setDB(std::unique_ptr<DatabaseManager> &database);
  void startMessageThread(void);
  void endMessageThread(void);
  void syncUIWithDB();

private slots:
  void on_StartButton_toggled(bool checked);

  void on_exitButton_clicked();

  void on_Add_clicked();

  void on_Edit_clicked();

  void on_Delete_clicked();

  void on_DeleteAll_clicked();

  void onMessageReceived(const QString &message);

signals:
  void startThread(bool);

private:
  QThread *workerThread; // Thread for handling native messaging communication
  NativeMessagingWorker *worker;
  Ui::MainWindow *ui;
  int numRows = 0;
  std::unique_ptr<DatabaseManager> db;
  std::unique_ptr<EncryptionUtil> enc;
  std::unique_ptr<Logger> logM;

  int getCurrRow();
  bool isValidDomain(const std::string &website);
  void startNativeMessagingThread();
};
#endif // MAINWINDOW_H
