#ifndef SETUP_H
#define SETUP_H

#include <QDialog>
#include <src/core/EncryptionUtil.h>
#include <src/core/DatabaseManager.h>

namespace Ui {
class Setup;
}

class Setup : public QDialog
{
    Q_OBJECT

public:
    explicit Setup(QWidget *parent = nullptr);
    ~Setup();

private slots:
    void on_SetupButton_clicked();

private:
    Ui::Setup *ui;
    EncryptionUtil* encdec = new EncryptionUtil();
    DatabaseManager* dbm = new DatabaseManager("db/passwords.db");
};

#endif // SETUP_H
