#ifndef NOTIFICATIONORDER_H
#define NOTIFICATIONORDER_H

#include "INotification.h"
#include "ListOrders.h"

class NotificationOrder : public QObject, public INotification
{
    Q_OBJECT
public:
    NotificationOrder();

    void drawNotification();
    void getRegisterInfo();

    static INotification *Initialize();

private:
    QSqlDatabase db;
    QTimer *timer;
    QString service;

    ListOrders *listOrders;

private slots:
    void checkTable();
};

#endif // NOTIFICATIONORDER_H
