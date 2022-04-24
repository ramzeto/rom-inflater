#include "maindialog.h"
#include "ui_maindialog.h"
#include "settings.h"

#include <QSettings>

#include <iostream>

MainDialog::MainDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MainDialog)
{
    ui->setupUi(this);
    //window()->setFixedSize(window()->sizeHint());

    ui->versionLabel->setText(QString("Version ") + APP_VERSION);
    //qDebug() << "QStandardPaths::AppDataLocation: " << QStandardPaths::displayName(QStandardPaths::AppDataLocation);
    //qDebug() << "QStandardPaths::HomeLocation: " << QStandardPaths::displayName(QStandardPaths::HomeLocation);

    QSettings settings;
    ui->cacheSpinBox->setValue(settings.value(SETTINGS_SIZE_CACHE, DEFAULT_SIZE_CACHE).toInt());
}

MainDialog::~MainDialog()
{
    delete ui;
}

void MainDialog::on_cacheSpinBox_valueChanged(int value)
{
    std::cout << __FUNCTION__ << " " << value << std::endl;

    QSettings settings;
    settings.setValue(SETTINGS_SIZE_CACHE, value);
}
