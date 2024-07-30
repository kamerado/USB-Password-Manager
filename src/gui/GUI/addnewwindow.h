#ifndef ADDNEWWINDOW_H
#define ADDNEWWINDOW_H

#include <QDialog>
#include <QListWidgetItem>
#include <QWidget>

namespace Ui {
class addnewwindow;
}

class addnewwindow : public QDialog
{
    Q_OBJECT

public:
    explicit addnewwindow(QWidget *parent = nullptr);
    ~addnewwindow();

private slots:
    void on_Add_clicked();

    void on_Edit_clicked();

    void on_Delete_clicked();

    void on_PasswordWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);

    void on_PasswordWidget_currentRowChanged(int currentRow);

private:
    Ui::addnewwindow *ui;

    int pwSelected = -1;
};

#endif // ADDNEWWINDOW_H
