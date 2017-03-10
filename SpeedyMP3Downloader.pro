QT       += core gui network webkit

TARGET = SpeedyMP3Downloader
TEMPLATE = app

SOURCES += main.cpp\
    mp3download.cpp \
    tabhistory.cpp \
    mainwindow.cpp \
    downloader.cpp \
    preferencesdialog.cpp \
    browsersettings.cpp \
    tabsearch.cpp

HEADERS  += mp3download.h \
    tabhistory.h \
    mainwindow.h \
    downloader.h \
    preferencesdialog.h \
    browsersettings.h \
    tabsearch.h

OTHER_FILES += \
    config.txt \
    publi.png \
    README.txt \
    LICENSE.txt \
    downloads.txt

RESOURCES += \
    files.qrc
