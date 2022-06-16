#ifndef INFLATEWINDOW_H
#define INFLATEWINDOW_H

#include <QMainWindow>
#include <QDir>
#include <QFile>
#include <QFileInfo>

#include "inflater.h"

namespace Ui {
class InflateWindow;
}

class InflateWindow : public QMainWindow
{
    Q_OBJECT
    void showEvent(QShowEvent *) override;
    void closeEvent(QCloseEvent *) override;
    void close();

public:
    explicit InflateWindow(QWidget *parent = nullptr);
    ~InflateWindow();

    void setCompressedRomFilename(const QString &value);
    void setCommand(const QString &value);
    void setExtensionsOrFilenames(const QStringList &value);

    void inflate();

private slots:
    void on_cancelPushButton_clicked();
    void onInflateProgress(const int &progress);
    void onInflateFinished(const int &result, const QString &message);

private:
    Ui::InflateWindow *ui;

    QString compressedRomFilename;
    QFile compressedRomFile;
    QFileInfo compressedRomFileInfo;
    QString command;
    QStringList extensionsOrFilenames;

    QDir romsDirectory;
    QDir romDirectory;
    QFileInfoList suitableFileInfoList;
    Inflater *inflater;
    bool isClosed;

    void clean();
    void launch();
    quint64 getDirectorySize(QDir directory);
    void queryRomDirectory(QDir directory);
    void makeM3uFile();
    void executeCommand(QFileInfo fileInfo);
};

#endif // INFLATEWINDOW_H
