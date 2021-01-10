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
    AddSparePart.cpp \
    Global.cpp \
    ListClients.cpp \
    ListEmployees.cpp \
    ListOrders.cpp \
    ListSpareParts.cpp \
    SettingsWindow.cpp \
    StartWindow.cpp \
    TaskList.cpp \
    UpdateClients.cpp \
    UpdateEmployee.cpp \
    UpdateOrders.cpp \
    UpdateSparePart.cpp \
    ViewClient.cpp \
    ViewEmployee.cpp \
    ViewOrders.cpp \
    ViewSparePart.cpp

RESOURCES += qml.qrc \
    images.qrc \
    translations.qrc

FORMS += \
    AddClient.ui \
    AddEmployee.ui \
    AddOrder.ui \
    AddSparePart.ui \
    ListClients.ui \
    ListEmployees.ui \
    ListOrders.ui \
    ListSpareParts.ui \
    SettingsWindow.ui \
    StartWindow.ui \
    TaskList.ui \
    UpdateClients.ui \
    UpdateEmployee.ui \
    UpdateOrders.ui \
    UpdateSparePart.ui \
    ViewClient.ui \
    ViewEmployee.ui \
    ViewOrders.ui \
    ViewSparePart.ui

HEADERS += \
    AddClient.h \
    AddEmployee.h \
    AddOrder.h \
    AddSparePart.h \
    Global.h \
    ListClients.h \
    ListEmployees.h \
    ListOrders.h \
    ListSpareParts.h \
    SettingsWindow.h \
    StartWindow.h \
    TaskList.h \
    UpdateClients.h \
    UpdateEmployee.h \
    UpdateOrders.h \
    UpdateSparePart.h \
    ViewClient.h \
    ViewEmployee.h \
    ViewOrders.h \
    ViewSparePart.h

CODECFORSRC = UTF-8
