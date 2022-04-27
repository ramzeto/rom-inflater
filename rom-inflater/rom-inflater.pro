QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    chooserdialog.cpp \
    inflater.cpp \
    inflatewindow.cpp \
    main.cpp \
    maindialog.cpp

HEADERS += \
    chooserdialog.h \
    inflater.h \
    inflatewindow.h \
    maindialog.h \
    settings.h

FORMS += \
    chooserdialog.ui \
    inflatewindow.ui \
    maindialog.ui

TRANSLATIONS += \
    rom-inflater_es_419.ts

VERSION = 1.0
DEFINES += APP_VERSION=\\\"$$VERSION\\\"

APP_ID = io.github.RomInflater
DEFINES += APP_ID=\\\"$$APP_ID\\\"

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

unix:!macx: LIBS += -larchive


# Default rules for deployment.
unix {
    isEmpty(PREFIX) {
        PREFIX = /usr
    }

    target.path = $$PREFIX/bin

    shortcutfiles.files = resources/io.github.RomInflater.desktop
    shortcutfiles.path = $$PREFIX/share/applications/
    data.files += resources/io.github.RomInflater.svg
    data.path = $$PREFIX/share/pixmaps/

    INSTALLS += shortcutfiles
    INSTALLS += data
}

INSTALLS += target

DISTFILES += \
    resources/qtquickapp.desktop \
    resources/qtquickapp.png
