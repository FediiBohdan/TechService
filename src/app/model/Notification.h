#ifndef NOTIFICATION_H
#define NOTIFICATION_H

#include "Global.h"

#include <QDate>
#include <QTime>
#include <QTimer>
#include <QDebug>
#include <QtWidgets>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QSqlQueryModel>

class Notification
{
public:
    Notification();

    virtual void drawNotification() = 0;
    virtual void getRegisterInfo() = 0;
};

#endif // NOTIFICATION_H
