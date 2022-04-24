#include "chooserdialog.h"
#include "ui_chooserdialog.h"

#include <iostream>


ChooserDialog::ChooserDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChooserDialog)
{
    ui->setupUi(this);
}

ChooserDialog::~ChooserDialog()
{
    delete ui;
}

void ChooserDialog::on_cancelPushButton_clicked()
{
    close();
}

void ChooserDialog::setFileInfoList(const QFileInfoList &value)
{
    fileInfoList = value;

    for(int c = 0; c < fileInfoList.size(); c++)
    {
        QFileInfo fileInfo = fileInfoList.at(c);
        new QListWidgetItem(fileInfo.fileName(), ui->listWidget);
    }
}

QFileInfo ChooserDialog::getFileInfo() const
{
    return fileInfo;
}

void ChooserDialog::on_listWidget_itemClicked(QListWidgetItem *item)
{
    std::cerr << "ChooserDialog::" << __FUNCTION__ << " " << item->text().toLocal8Bit().constData() << std::endl;
    fileInfo = fileInfoList.at(ui->listWidget->row(item));

    close();
}

