#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include <src/core/EncryptionUtil.h>
#include <src/core/DatabaseManager.h>
#include <src/core/Logger.h>

namespace Ui {
class Login;
}

class Login : public QDialog
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);
    explicit Login(Logger* logM, QWidget *parent = nullptr);
    void setDB(DatabaseManager* database);
    void getEnc(void);
    ~Login();

private slots:
    void on_LoginButton_clicked();

signals:
    void sendEnc(EncryptionUtil* val);

private:
    Ui::Login *ui;
    EncryptionUtil* encdec;
    DatabaseManager* db;
    Logger* logM;
};

#endif // LOGIN_H
