#include "inputdialog.h"
#include "ui_inputdialog.h"

InputDialog::InputDialog(QWidget *parent)
    : QDialog(parent), ui(new Ui::InputDialog) {
  ui->setupUi(this);
}

InputDialog::~InputDialog() { delete ui; }

QString InputDialog::getText() const { return ui->inputLineEdit->text(); }

void InputDialog::on_buttonBox_accepted()
{

}

