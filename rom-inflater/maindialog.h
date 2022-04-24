#ifndef MAINDIALOG_H
#define MAINDIALOG_H

#include <QDialog>

namespace Ui {
class MainDialog;
}

class MainDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MainDialog(QWidget *parent = nullptr);
    ~MainDialog();

private slots:
    void on_cacheSpinBox_valueChanged(int arg1);

private:
    Ui::MainDialog *ui;
};

#endif // MAINDIALOG_H
