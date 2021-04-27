#ifndef NOTIFICATIONORDER_H
#define NOTIFICATIONORDER_H

#include "Notification.h"
#include "ListOrders.h"

class NotificationOrder : public QObject, public Notification
{
    Q_OBJECT
public:
    NotificationOrder();

    void getRegisterInfo();
    void drawNotification();    

    static Notification *Initialize();

private:
    QSqlDatabase db;

    QTimer *timer;
    ListOrders *listOrders;

    QString service;    

private slots:
    void checkTable();
};

#endif // NOTIFICATIONORDER_H
