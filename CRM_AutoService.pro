TEMPLATE = app

QT += core
QT += qml
QT += quick
QT += gui
QT += network
QT += widgets

CONFIG += c++11

SOURCES += main.cpp \
    OrderCreation.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

FORMS += \
    OrderCreation.ui

HEADERS += \
    OrderCreation.h

