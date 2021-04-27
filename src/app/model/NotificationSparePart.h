#ifndef NOTIFICATIONSPAREPART_H
#define NOTIFICATIONSPAREPART_H

#include "Notification.h"

class NotificationSparePart : public QObject, public Notification
{
    Q_OBJECT
public:
    NotificationSparePart();

    void getRegisterInfo();
    void drawNotification();

    static Notification *Initialize();

private:
    QSqlDatabase db;

    QTimer *timer;

    QString creator;
    QString content;
    QString position;

    QPointer<QSqlQueryModel> queryCheckOrderUpdate;

private slots:
    void checkTable();
};

#endif // NOTIFICATIONSPAREPART_H
