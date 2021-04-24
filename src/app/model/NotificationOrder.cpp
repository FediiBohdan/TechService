#include "NotificationOrder.h"

NotificationOrder::NotificationOrder()
{
    timer = new QTimer(this);
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(checkTable()));
    timer->start(60000);

    getRegisterInfo();
}

Notification *NotificationOrder::Initialize()
{
    NotificationOrder *notificationOrder = new NotificationOrder();
    return notificationOrder;
}

void NotificationOrder::getRegisterInfo()
{
    service = global::getSettingsValue("userService", "settings").toString();
}

void NotificationOrder::checkTable()
{
    QSqlQuery queryCheckOrderUpdate(db);

    QDate currentDate = QDate::currentDate();
    QTime currentTime = QTime::currentTime().addSecs(-60);

    queryCheckOrderUpdate.prepare("SELECT creation_date, creation_time FROM orders_history WHERE (creation_date = '" + currentDate.toString(Qt::SystemLocaleDate) + "' "
                                  "AND creation_time >= '" + currentTime.toString(Qt::SystemLocaleDate) + "' AND service_address = '" + service + "')");
    queryCheckOrderUpdate.exec();

    if (queryCheckOrderUpdate.first() == true)
        drawNotification();
}

void NotificationOrder::drawNotification()
{
    QMessageBox msgBox;
    msgBox.setText(tr("У сервиса на %1 появился новый заказ!").arg(service));
    QAbstractButton *openOrdersButton = msgBox.addButton(tr("Открыть список заказов"), QMessageBox::YesRole);
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.exec();

    if (msgBox.clickedButton() == openOrdersButton)
    {
        listOrders = new ListOrders;
        listOrders->show();
        listOrders->setAttribute(Qt::WA_DeleteOnClose);
    }
}
