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
    ClientTable.cpp \
    OrderCreation.cpp \
    SparePartsTable.cpp \
    StaffTable.cpp \
    StartWindow.cpp

RESOURCES += qml.qrc \
    images.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

FORMS += \
    ClientTable.ui \
    OrderCreation.ui \
    SparePartsTable.ui \
    StaffTable.ui \
    StartWindow.ui

HEADERS += \
    ClientTable.h \
    OrderCreation.h \
    SparePartsTable.h \
    StaffTable.h \
    StartWindow.h

DISTFILES +=

