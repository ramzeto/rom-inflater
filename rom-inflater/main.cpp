#include "maindialog.h"
#include "inflatewindow.h"
#include "chooserdialog.h"
#include "settings.h"

#include <QApplication>
#include <QStandardPaths>
#include <QDir>
#include <QSettings>

#include <iostream>

// https://stackoverflow.com/questions/3492739/auto-expanding-layout-with-qt-designer
// https://amin-ahmadi.com/2016/09/26/how-to-get-current-app-version-in-qt/

int main(int argc, char *argv[])
{
    // io.github.RomInflater

    //QCoreApplication::setOrganizationName(APP_ID);
    //QCoreApplication::setOrganizationDomain(APP_ID);
    QCoreApplication::setApplicationName(APP_ID);

    std::cout << "QStandardPaths::AppDataLocation: " << QStandardPaths::writableLocation(QStandardPaths::AppDataLocation).toLocal8Bit().constData() << std::endl;
    std::cout << "QStandardPaths::HomeLocation: " << QStandardPaths::writableLocation(QStandardPaths::HomeLocation).toLocal8Bit().constData() << std::endl;

    QDir appDataDir(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
    if(!appDataDir.exists())
    {
        appDataDir.mkdir(appDataDir.absolutePath());
    }

    QDir romsDir(appDataDir.absolutePath() + DIRECTORY_ROMS);
    if(!romsDir.exists())
    {
        romsDir.mkdir(romsDir.absolutePath());
    }

    // Help
    if(argc == 2)
    {
        if(!QString(argv[1]).compare("-h") || !QString(argv[1]).compare("--help"))
        {
            std::cout << "help" << std::endl;
        }

        return EXIT_SUCCESS;
    }
    // Inflate
    else if(argc >= 4)
    {
        QString compressedRomFilename = QString(argv[1]);
        QString command = QString(argv[argc - 1]);
        QStringList extensionsOrFilenames;

        for(int c = 2; c < argc - 1; c++)
        {
            extensionsOrFilenames.append(QString(argv[c]));
        }

        std::cout << "compressedRomFilename: " << compressedRomFilename.toLocal8Bit().constData() << std::endl;
        std::cout << "command: " << command.toLocal8Bit().constData() << std::endl;
        for(int c = 0; c < extensionsOrFilenames.size(); c++)
        {
            std::cout << "extensionsOrFilename: " << extensionsOrFilenames.at(c).toLocal8Bit().constData() << std::endl;
        }

        QApplication a(argc, argv);

        InflateWindow inflateWindow;
        inflateWindow.setCompressedRomFilename(compressedRomFilename);
        inflateWindow.setCommand(command);
        inflateWindow.setExtensionsOrFilenames(extensionsOrFilenames);
        inflateWindow.show();

        return a.exec();
    }

    QApplication a(argc, argv);
    MainDialog mainDialog;
    mainDialog.show();

    return a.exec();
}
