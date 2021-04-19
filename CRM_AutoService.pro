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
    AddTask.cpp \
    AnalyticsDialog.cpp \
    Global.cpp \
    ListClients.cpp \
    ListEmployees.cpp \
    ListOrders.cpp \
    ListSpareParts.cpp \
    ListTasks.cpp \
    Notification.cpp \
    NotificationOrder.cpp \
    SettingsWindow.cpp \
    StartWindow.cpp \
    ViewUpdateClient.cpp \
    ViewUpdateEmployee.cpp \
    ViewUpdateOrder.cpp \
    ViewUpdateSparePart.cpp \
    ViewUpdateTask.cpp \
    notificationsparepart.cpp

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
    ViewUpdateClient.ui \
    ViewUpdateEmployee.ui \
    ViewUpdateOrder.ui \
    ViewUpdateSparePart.ui \
    ViewUpdateTask.ui

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
    Notification.h \
    NotificationOrder.h \
    SettingsWindow.h \
    StartWindow.h \
    ViewUpdateClient.h \
    ViewUpdateEmployee.h \
    ViewUpdateOrder.h \
    ViewUpdateSparePart.h \
    ViewUpdateTask.h \
    notificationsparepart.h

RESOURCES += qml.qrc \
    images.qrc \
    translations.qrc

CODECFORSRC = UTF-8

DISTFILES +=
