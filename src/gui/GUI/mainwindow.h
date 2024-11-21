#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "src/core/DatabaseManager.h"
#include "src/core/EncryptionUtil.h"
#include "src/core/WebSocket.h"
#include <memory>
#include <qglobal.h>
#include <qpushbutton.h>
#include <qwebsocket.h>
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
  MainWindow(std::shared_ptr<Logger> &logM, QWidget *parent = nullptr);
  ~MainWindow();
  void closeEvent(QCloseEvent *event) override;
  void setEnc(std::unique_ptr<EncryptionUtil> &encddec);
  void setDB(std::unique_ptr<DatabaseManager> &database);
  void syncUIWithDB();
  void startServer();
  void stopServer();
  // void waitForServerStop();

private slots:
  void on_StartButton_toggled(bool checked);
  void on_exitButton_clicked();
  void on_Add_clicked();
  void on_Edit_clicked();
  void on_Delete_clicked();
  void on_DeleteAll_clicked();
  void on_SettingsButton_clicked();
public slots:
  void parseMessage(const QString &, const QWebSocket *client);

private:
  Ui::MainWindow *ui;
  // std::unique_ptr<QPushButton> startbtn;
  std::unique_ptr<WebSocketServer> server = nullptr;
  void receiveToggleSignal(bool &);

  int numRows = 0;
  std::unique_ptr<DatabaseManager> db;
  std::unique_ptr<EncryptionUtil> enc;
  std::shared_ptr<Logger> logger;

  int getCurrRow();
  bool isValidDomain(const std::string &website);
  bool isOpen;
};
#endif // MAINWINDOW_H
