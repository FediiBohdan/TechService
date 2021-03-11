TEMPLATE = app

QT += core
QT += qml
QT += quick
QT += gui
QT += network
QT += widgets
QT += sql
QT += charts

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
    AddTask.cpp \
    AnalyticsDialog.cpp \
    Global.cpp \
    ListClients.cpp \
    ListEmployees.cpp \
    ListOrders.cpp \
    ListSpareParts.cpp \
    ListTasks.cpp \
    SettingsWindow.cpp \
    StartWindow.cpp \
    UpdateEmployee.cpp \
    UpdateOrders.cpp \
    UpdateSparePart.cpp \
    UpdateTask.cpp \
    ViewEmployee.cpp \
    ViewOrders.cpp \
    ViewSparePart.cpp \
    ViewTask.cpp \
    ViewUpdateClient.cpp

RESOURCES += qml.qrc \
    images.qrc \
    translations.qrc

FORMS += \
    AddClient.ui \
    AddEmployee.ui \
    AddOrder.ui \
    AddSparePart.ui \
    AddTask.ui \
    AnalyticsDialog.ui \
    ListClients.ui \
    ListEmployees.ui \
    ListOrders.ui \
    ListSpareParts.ui \
    ListTasks.ui \
    SettingsWindow.ui \
    StartWindow.ui \
    UpdateEmployee.ui \
    UpdateOrders.ui \
    UpdateSparePart.ui \
    UpdateTask.ui \
    ViewEmployee.ui \
    ViewOrders.ui \
    ViewSparePart.ui \
    ViewTask.ui \
    ViewUpdateClient.ui

HEADERS += \
    AddClient.h \
    AddEmployee.h \
    AddOrder.h \
    AddSparePart.h \
    AddTask.h \
    AnalyticsDialog.h \
    Global.h \
    ListClients.h \
    ListEmployees.h \
    ListOrders.h \
    ListSpareParts.h \
    ListTasks.h \
    SettingsWindow.h \
    StartWindow.h \
    UpdateEmployee.h \
    UpdateOrders.h \
    UpdateSparePart.h \
    UpdateTask.h \
    ViewEmployee.h \
    ViewOrders.h \
    ViewSparePart.h \
    ViewTask.h \
    ViewUpdateClient.h

CODECFORSRC = UTF-8

DISTFILES +=
