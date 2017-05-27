QT += core
QT += widgets
QT += network

CONFIG += c++11
CONFIG += console

SOURCES += main.cpp \
    filedownloader.cpp \
    downloadwindow.cpp \
    mainwindow.cpp \
    commandwindow.cpp \
    commandtext.cpp

HEADERS += \
    filedownloader.h \
    downloadwindow.h \
    mainwindow.h \
    commandwindow.h \
    commandtext.h

DISTFILES -= \
    listFiles

RESOURCES += \
    resources.qrc
