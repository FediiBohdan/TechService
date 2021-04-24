#ifndef NOTIFICATIONORDER_H
#define NOTIFICATIONORDER_H

#include "Notification.h"
#include "ListOrders.h"

class NotificationOrder : public QObject, public Notification
{
    Q_OBJECT
public:
    NotificationOrder();

    void drawNotification();
    void getRegisterInfo();

    static Notification *Initialize();

private:
    QSqlDatabase db;
    QTimer *timer;
    QString service;

    ListOrders *listOrders;

private slots:
    void checkTable();
};

#endif // NOTIFICATIONORDER_H
