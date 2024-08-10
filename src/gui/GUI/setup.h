#ifndef SETUP_H
#define SETUP_H

#include <QDialog>
#include <src/core/EncryptionUtil.h>
#include <src/core/DatabaseManager.h>
#include <src/core/Logger.h>

namespace Ui {
class Setup;
}

class Setup : public QDialog
{
    Q_OBJECT

public:
    explicit Setup(QWidget *parent = nullptr);  // Default constructor
    explicit Setup(Logger* logM, QWidget *parent = nullptr);  // Default constructor
    void setDB(DatabaseManager* database);
    void getEnc(void);
    ~Setup();

private slots:
    void on_SetupButton_clicked();

signals:
    void sendEnc(EncryptionUtil* val);

private:
    Ui::Setup *ui;
    EncryptionUtil* encdec;
    DatabaseManager* db;
    Logger* logM;
};

#endif // SETUP_H
