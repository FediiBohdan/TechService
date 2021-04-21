#ifndef INOTIFICATION_H
#define INOTIFICATION_H

#include "Global.h"

#include <QDate>
#include <QTime>
#include <QTimer>
#include <QDebug>
#include <QtWidgets>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QSqlQueryModel>

class INotification
{
public:
    INotification();

    virtual void drawNotification() = 0;
    virtual void getRegisterInfo() = 0;
};

#endif // INOTIFICATION_H
