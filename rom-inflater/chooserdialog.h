#ifndef CHOOSERDIALOG_H
#define CHOOSERDIALOG_H

#include <QDialog>
#include <QFileInfoList>
#include <QListWidgetItem>

namespace Ui {
class ChooserDialog;
}

class ChooserDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ChooserDialog(QWidget *parent = nullptr);
    ~ChooserDialog();

    void setFileInfoList(const QFileInfoList &value);
    QFileInfo getFileInfo() const;

private slots:
    void on_listWidget_itemClicked(QListWidgetItem *item);

    void on_cancelPushButton_clicked();

private:
    Ui::ChooserDialog *ui;

    QFileInfoList fileInfoList;
    QFileInfo fileInfo;
};

#endif // CHOOSERDIALOG_H
