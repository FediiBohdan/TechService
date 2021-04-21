#include "NotificationSparePart.h"

NotificationSparePart::NotificationSparePart()
{
    timer = new QTimer(this);
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(checkTable()));
    timer->start(60000);

    getRegisterInfo();
}

INotification *NotificationSparePart::Initialize()
{
    NotificationSparePart *notificationOrder = new NotificationSparePart();
    return notificationOrder;
}

void NotificationSparePart::getRegisterInfo()
{
    position = global::getSettingsValue("userPosition", "settings").toString();
}

void NotificationSparePart::checkTable()
{
    QSqlQuery queryCheckOrderUpdate(db);

    QDate currentDate = QDate::currentDate();
    QTime currentTime = QTime::currentTime().addSecs(-60);

    queryCheckOrderUpdate.prepare("SELECT creator, content, date_creation, time_creation FROM order_notification WHERE (date_creation = '" + currentDate.toString(Qt::SystemLocaleDate) + "' "
                                  "AND time_creation >= '" + currentTime.toString(Qt::SystemLocaleDate) + "')");
    queryCheckOrderUpdate.exec();
    queryCheckOrderUpdate.next();

    if ((queryCheckOrderUpdate.first() == true) && (position == "Менеджер"))
    {
        creator = queryCheckOrderUpdate.value(0).toString();
        content = queryCheckOrderUpdate.value(1).toString();
        drawNotification();
    }
}

void NotificationSparePart::drawNotification()
{
    QMessageBox msgBox;
    msgBox.setTextFormat(Qt::RichText);
    QString title = "<h3>" + tr("Пользователь с логином %1 создал заявку!").arg(creator) + "</h3>";
    msgBox.setText((title));
    msgBox.setInformativeText(content);
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.exec();
}
