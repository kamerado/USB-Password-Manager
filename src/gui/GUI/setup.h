#ifndef SETUP_H
#define SETUP_H

#include <QDialog>
#include <src/core/DatabaseManager.h>
#include <src/core/EncryptionUtil.h>
#include <src/core/Logger.h>

namespace Ui {
class Setup;
}

class Setup : public QDialog {
  Q_OBJECT

public:
  explicit Setup(QWidget *parent = nullptr); // Default constructor
  explicit Setup(std::shared_ptr<Logger *> &logM,
                 QWidget *parent = nullptr); // Default constructor
  void setDB(std::unique_ptr<DatabaseManager> &database);
  void getEnc(void);
  ~Setup();

private slots:
  void on_SetupButton_clicked();

signals:
  void sendEnc(std::unique_ptr<EncryptionUtil> &val);

private:
  Ui::Setup *ui;
  std::unique_ptr<EncryptionUtil> encdec;
  std::unique_ptr<DatabaseManager> db;
  std::shared_ptr<Logger *> logM;
};

#endif // SETUP_H
