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
// https://docs.flatpak.org/en/latest/available-runtimes.html

// https://community.kde.org/Guidelines_and_HOWTOs/Flatpak
// https://docs.flatpak.org/en/latest/qt.html

// https://docs.flatpak.org/en/latest/first-build.html
// https://docs.flatpak.org/en/latest/freedesktop-quick-reference.html#appdata-files

// https://docs.appimage.org/packaging-guide/from-source/native-binaries.html

int main(int argc, char *argv[])
{
    // io.github.RomInflater

    //QCoreApplication::setOrganizationName(APP_ID);
    //QCoreApplication::setOrganizationDomain(APP_ID);
    QCoreApplication::setApplicationName(APP_ID);

    //std::cout << "QStandardPaths::AppDataLocation: " << QStandardPaths::writableLocation(QStandardPaths::AppDataLocation).toLocal8Bit().constData() << std::endl;
    //std::cout << "QStandardPaths::HomeLocation: " << QStandardPaths::writableLocation(QStandardPaths::HomeLocation).toLocal8Bit().constData() << std::endl;

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
            std::cout << "Usage:" << std::endl;
            std::cout << "\t" << "RomInflater-x86_64.AppImage <compressedRom> <fileOrExt1 [fileOrExtN]> <\"command %INFLATEDFILE%\">" << std::endl;
            std::cout << std::endl;
            std::cout << std::endl;
            std::cout << "compressedRom" << std::endl << "\t" << "Filename of the compressed ROM (Zip and 7z formats are supported)." << std::endl;
            std::cout << std::endl;
            std::cout << "fileOrExt 1...N" << std::endl << "\t" << "Extensions or filenames to look up after inflating (For example: .iso, .cue, EBOOT.bin)." << std::endl;
            std::cout << std::endl;
            std::cout << "command" << std::endl << "\t" << "Command that will be executed after inflating." << std::endl;
            std::cout << std::endl;
            std::cout << "%INFLATEDFILE%" << std::endl << "\t" << "Will be replaced by the inflated filename. If more than one suitable file is found, a chooser will be displayed." << std::endl;
            std::cout << std::endl;
            std::cout << "If multiple .cue files are found, a .m3u file will be automatically generated with the .cue files listed in order, in case the ROM is to be used with the Beetle Saturn core in Retroarch." << std::endl;
            std::cout << std::endl;
        }
        else if(!QString(argv[1]).compare("--version"))
        {
            std::cout << "Version: " << APP_VERSION << std::endl;
            std::cout << std::endl;
        }
        else
        {
            std::cerr << "Unknown option" << std::endl;
            std::cerr << std::endl;
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
