#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include <memory>
#include <src/core/DatabaseManager.h>
#include <src/core/EncryptionUtil.h>
#include <src/core/Logger.h>

namespace Ui {
class Login;
}

class Login : public QDialog {
  Q_OBJECT

public:
  explicit Login(QWidget *parent = nullptr);
  explicit Login(std::shared_ptr<Logger> &logM, QWidget *parent = nullptr);
  void setDB(std::unique_ptr<DatabaseManager> &database);
  void getEnc(void);
  ~Login();

private slots:
  void on_LoginButton_clicked();

signals:
  void sendEnc(std::unique_ptr<EncryptionUtil> &val);

private:
  Ui::Login *ui;
  std::unique_ptr<EncryptionUtil> encdec;
  std::unique_ptr<DatabaseManager> db;
  std::shared_ptr<Logger> logM;
};

#endif // LOGIN_H
