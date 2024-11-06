#ifndef INPUTDIALOG_H
#define INPUTDIALOG_H

#include <QDialog>

namespace Ui {
class InputDialog;
}

class InputDialog : public QDialog {
  Q_OBJECT

public:
  explicit InputDialog(QWidget *parent = nullptr);
  ~InputDialog();

  QString getText() const;

private:
  Ui::InputDialog *ui;
};

#endif // INPUTDIALOG_H
