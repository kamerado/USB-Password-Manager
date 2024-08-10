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
    explicit Setup(QWidget *parent = nullptr);  // Default constructor
    explicit Setup(const char* dbPath, QWidget *parent = nullptr);
    void getEnc(void);
    ~Setup();

private slots:
    void on_SetupButton_clicked();

signals:
    void sendEnc(EncryptionUtil* val);

private:
    Ui::Setup *ui;
    EncryptionUtil* encdec;
    DatabaseManager* dbm;
};

#endif // SETUP_H
