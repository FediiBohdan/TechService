TEMPLATE = app

QT += core
QT += qml
QT += quick
QT += gui
QT += network
QT += widgets
QT += sql

VERSION = 1.0.0.0

win32
{
   RC_ICONS = $$PWD/images/autoService-logo.ico
}

CONFIG += c++11

SOURCES += main.cpp \
    AddWorker.cpp \
    ClientTable.cpp \
    OrderCreation.cpp \
    SettingsWindow.cpp \
    SparePartsTable.cpp \
    StaffTable.cpp \
    StartWindow.cpp

RESOURCES += qml.qrc \
    images.qrc \
    translations.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

FORMS += \
    AddWorker.ui \
    ClientTable.ui \
    OrderCreation.ui \
    SettingsWindow.ui \
    SparePartsTable.ui \
    StaffTable.ui \
    StartWindow.ui

HEADERS += \
    AddWorker.h \
    ClientTable.h \
    OrderCreation.h \
    SettingsWindow.h \
    SparePartsTable.h \
    StaffTable.h \
    StartWindow.h

DISTFILES +=

