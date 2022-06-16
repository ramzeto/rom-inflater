#include "inflatewindow.h"
#include "ui_inflatewindow.h"
#include "chooserdialog.h"
#include "settings.h"

#include <QMessageBox>
#include <QFile>
#include <QStandardPaths>
#include <QSettings>
#include <QTimer>
#include <QCloseEvent>
#include <QTextStream>

#include <iostream>

InflateWindow::InflateWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::InflateWindow)
{
    ui->setupUi(this);

    //window()->setFixedSize(window()->sizeHint());

    inflater = NULL;
    isClosed = false;

    QDir appDataDir(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
    romsDirectory = QDir(appDataDir.absolutePath() + DIRECTORY_ROMS);

    ui->progressBar->setValue(0);
    ui->versionLabel->setText(QString("Version ") + APP_VERSION);
}

InflateWindow::~InflateWindow()
{
    delete ui;
}

void InflateWindow::showEvent(QShowEvent *)
{
    QTimer::singleShot(200, this, [this]() -> void{
        inflate();
    });
}

void InflateWindow::closeEvent(QCloseEvent *event)
{
    if(isClosed)
    {
        event->accept();
        return;
    }
    event->ignore();
}

void InflateWindow::close()
{
    isClosed = true;
    QMainWindow::close();
}

void InflateWindow::on_cancelPushButton_clicked()
{
    ui->cancelPushButton->setEnabled(false);
    inflater->cancel();
}

void InflateWindow::onInflateProgress(const int &progress)
{
    ui->progressBar->setValue(progress);
}

void InflateWindow::onInflateFinished(const int &result, const QString &message)
{
    std::cout << __FUNCTION__ << " result: " << result << " message: " << message.toLocal8Bit().constData() << std::endl;

    if(result == Inflater::RESULT_OK)
    {
        clean();
        launch();
    }
    else if(result == Inflater::RESULT_ERROR)
    {
        romDirectory.removeRecursively();

        QMessageBox messageBox;
        messageBox.setText(message);
        messageBox.exec();

        close();
    }
    else if(result == Inflater::RESULT_CANCELED)
    {
        romDirectory.removeRecursively();
        close();
    }
}

void InflateWindow::setCompressedRomFilename(const QString &value)
{
    compressedRomFilename = value;
    ui->compressedRomLabel->setText(compressedRomFilename);

    compressedRomFile.setFileName(compressedRomFilename);
    compressedRomFileInfo.setFile(compressedRomFile);
}

void InflateWindow::setExtensionsOrFilenames(const QStringList &value)
{
    extensionsOrFilenames = value;
}

void InflateWindow::setCommand(const QString &value)
{
    command = value;
}

void InflateWindow::inflate()
{    
    std::cout << __FUNCTION__ << " compressedRomFileInfo: " << compressedRomFileInfo.fileName().toLocal8Bit().constData() << std::endl;

    romDirectory = QDir(romsDirectory.absolutePath() + "/" + compressedRomFileInfo.fileName().replace(".", "_").replace(" ", "_"));
    std::cout << __FUNCTION__ << " romDirectory: " << romDirectory.absolutePath().toLocal8Bit().constData() << std::endl;

    // Check if files are in place
    if(romDirectory.exists())
    {
        clean();
        launch();
        return;
    }

    romDirectory.mkdir(romDirectory.absolutePath());

    inflater = new Inflater(this, romDirectory.absolutePath(), compressedRomFilename);
    connect(inflater, &Inflater::onProgress, this, &InflateWindow::onInflateProgress);
    connect(inflater, &Inflater::onFinished, this, &InflateWindow::onInflateFinished);
    connect(inflater, &Inflater::finished, inflater, &QObject::deleteLater);

    inflater->start();
}

void InflateWindow::clean()
{
    QSettings settings;
    quint64 maxCacheSize = settings.value(SETTINGS_SIZE_CACHE, DEFAULT_SIZE_CACHE).toInt() * 1000 * 1000 * 1000;
    quint64 cacheSize = getDirectorySize(romsDirectory);

    std::cout << __FUNCTION__ << " maxCacheSize: " << maxCacheSize << " cacheSize: " << cacheSize << std::endl;

    if(cacheSize > maxCacheSize)
    {
        quint64 cacheSizeCleared = 0;
        quint64 cacheSizeToClear = cacheSize - maxCacheSize;

        QFileInfoList entryInfoList = romsDirectory.entryInfoList(QDir::NoDotAndDotDot | QDir::AllDirs | QDir::Files, QDir::Time);
        for(int c = entryInfoList.size() - 1; c >= 1; c--)
        {
            quint64 size;
            QFileInfo fileInfo = entryInfoList.at(c);
            if(fileInfo.isDir())
            {
                QDir directory(QDir(fileInfo.absoluteFilePath()));
                size = getDirectorySize(directory);
                directory.removeRecursively();
            }
            else
            {
                size = fileInfo.size();
                QFile file(fileInfo.absoluteFilePath());
                file.remove();
            }
            std::cout << __FUNCTION__ << " " << fileInfo.absoluteFilePath().toLocal8Bit().constData() << " size: " << size << std::endl;

            cacheSizeCleared += size;
            if(cacheSizeCleared >= cacheSizeToClear)
            {
                break;
            }
        }
    }
}

void InflateWindow::launch()
{
    queryRomDirectory(romDirectory);
    makeM3uFile();

    if(suitableFileInfoList.size() == 0)
    {
        romDirectory.removeRecursively();

        QMessageBox messageBox;
        messageBox.setText("No suitable files where found");
        messageBox.exec();

        close();
    }
    else if(suitableFileInfoList.size() > 1)
    {
        ChooserDialog chooserDialog(this);
        chooserDialog.setFileInfoList(suitableFileInfoList);
        chooserDialog.exec();

        if(chooserDialog.getFileInfo().absoluteFilePath().length() == 0)
        {
            close();
            return;
        }
        executeCommand(chooserDialog.getFileInfo());
    }
    else
    {
        executeCommand(suitableFileInfoList.at(0));
    }
}

quint64 InflateWindow::getDirectorySize(QDir directory)
{
    quint64 size = 0;
    QFileInfoList entryInfoList = directory.entryInfoList(QDir::NoDotAndDotDot | QDir::AllDirs | QDir::Files);
    for(int c = 0; c < entryInfoList.size(); c++)
    {
        QFileInfo fileInfo = entryInfoList.at(c);
        if(fileInfo.isFile())
        {
            size += (quint64)fileInfo.size();
        }
        else if(fileInfo.isDir())
        {
            size += getDirectorySize(QDir(fileInfo.absoluteFilePath()));
        }
    }
    return size;
}

void InflateWindow::queryRomDirectory(QDir directory)
{
    QFileInfoList entryInfoList = directory.entryInfoList(QDir::NoDotAndDotDot | QDir::AllDirs | QDir::Files);
    for(int c = 0; c < entryInfoList.size(); c++)
    {
        QFileInfo fileInfo = entryInfoList.at(c);
        if(fileInfo.isFile())
        {
            for(int x = 0; x < extensionsOrFilenames.size(); x++)
            {
                QString item = extensionsOrFilenames.at(x);

                // Extension
                if(item.startsWith("."))
                {
                    if(fileInfo.fileName().endsWith(item, Qt::CaseInsensitive))
                    {
                        std::cout << __FUNCTION__ << " Extension: " << item.toLocal8Bit().constData() << std::endl;
                        suitableFileInfoList.append(fileInfo);
                    }
                }
                // Filename
                else if(!item.compare(fileInfo.fileName(), Qt::CaseInsensitive))
                {
                    std::cout << __FUNCTION__ << " Filename: " << item.toLocal8Bit().constData() << std::endl;
                    suitableFileInfoList.append(fileInfo);
                }
            }
        }
        else if(fileInfo.isDir())
        {
            queryRomDirectory(QDir(fileInfo.absoluteFilePath()));
        }

        std::cout << __FUNCTION__ << " " << fileInfo.absoluteFilePath().toLocal8Bit().constData() << std::endl;
    }
}

void InflateWindow::makeM3uFile()
{
    QFileInfoList cueFileInforList;
    for(int c = 0; c < suitableFileInfoList.size(); c++)
    {
        QFileInfo fileInfo = suitableFileInfoList.at(c);
        if(fileInfo.absoluteFilePath().endsWith(".cue"))
        {
            cueFileInforList.append(fileInfo);
        }
    }

    if(cueFileInforList.size() > 1)
    {
        std::cout << __FUNCTION__ << " " << (romDirectory.absolutePath() + "/" + compressedRomFileInfo.baseName() + ".m3u").toLocal8Bit().constData() << std::endl;

        QFile m3uFile(romDirectory.absolutePath() + "/" + compressedRomFileInfo.baseName() + ".m3u");
        if (!m3uFile.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            return;
        }
        QTextStream m3uTextStream(&m3uFile);
        for(int c = 0; c < cueFileInforList.size(); c++)
        {
            QFileInfo fileInfo = cueFileInforList.at(c);
            m3uTextStream << fileInfo.absoluteFilePath() << "\n";
        }
        m3uFile.close();

        suitableFileInfoList.prepend(QFileInfo(m3uFile));
    }
}

void InflateWindow::executeCommand(QFileInfo fileInfo)
{
    std::cout << __FUNCTION__ << " " << fileInfo.absoluteFilePath().toLocal8Bit().constData() << std::endl;

    command = command.replace("%INFLATEDFILE%", "\"" + fileInfo.absoluteFilePath() + "\"");

    std::cout << __FUNCTION__ << " " << command.toLocal8Bit().constData() << std::endl;
    system(command.toLocal8Bit().constData());
    close();
}
