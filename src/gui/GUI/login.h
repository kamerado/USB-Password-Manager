#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include <src/core/EncryptionUtil.h>

namespace Ui {
class Login;
}

class Login : public QDialog
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);
    ~Login();

private slots:
    void on_LoginButton_clicked();

private:
    Ui::Login *ui;
    EncryptionUtil* encdec = new EncryptionUtil();
};

#endif // LOGIN_H
