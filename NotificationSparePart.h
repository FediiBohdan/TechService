#ifndef NOTIFICATIONSPAREPART_H
#define NOTIFICATIONSPAREPART_H

#include "INotification.h"

class NotificationSparePart : public QObject, public INotification
{
    Q_OBJECT
public:
    NotificationSparePart();

    void drawNotification();
    void getRegisterInfo();

    static INotification *Initialize();

private:
    QSqlDatabase db;
    QTimer *timer;
    QString position;
    QString creator;
    QString content;

    QPointer<QSqlQueryModel> queryCheckOrderUpdate;

private slots:
    void checkTable();
};

#endif // NOTIFICATIONSPAREPART_H
