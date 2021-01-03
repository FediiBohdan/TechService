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
    AddClient.cpp \
    AddEmployee.cpp \
    AddOrder.cpp \
    Global.cpp \
    ListClients.cpp \
    ListEmployees.cpp \
    ListOrders.cpp \
    ListSpareParts.cpp \
    SettingsWindow.cpp \
    StartWindow.cpp

RESOURCES += qml.qrc \
    images.qrc \
    translations.qrc

FORMS += \
    AddClient.ui \
    AddEmployee.ui \
    AddOrder.ui \
    ListClients.ui \
    ListEmployees.ui \
    ListOrders.ui \
    ListSpareParts.ui \
    SettingsWindow.ui \
    StartWindow.ui

HEADERS += \
    AddClient.h \
    AddEmployee.h \
    AddOrder.h \
    Global.h \
    ListClients.h \
    ListEmployees.h \
    ListOrders.h \
    ListSpareParts.h \
    SettingsWindow.h \
    StartWindow.h

CODECFORSRC = UTF-8
