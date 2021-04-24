#include "Global.h"
#include "AddOrder.h"
#include "StartWindow.h"
#include "Notification.h"
#include "NotificationOrder.h"
#include "NotificationSparePart.h"

#include <QDir>
#include <QDebug>
#include <QObject>
#include <QLockFile>
#include <QTranslator>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    app.setApplicationName("TechService CRM");
    app.setOrganizationName("TechService");
    app.setApplicationVersion("1.0.0.0");

    QLockFile lockFile(QDir::temp().absoluteFilePath("lockFile.lock"));

    if (!lockFile.tryLock(100))
    {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setText(QObject::tr("Приложение уже запущено!"));
        msgBox.exec();
        return 1;
    }

    // translations connection
    QString registerLanguage = global::getSettingsValue("language", "settings").toString();
    QTranslator translator;

    if (registerLanguage == "Русский")
    {
        translator.load(":/translations/russian.qm");
        app.installTranslator(&translator);
    }
    else if (registerLanguage == "Українська")
    {
        translator.load(":/translations/ukrainian.qm");
        app.installTranslator(&translator);
    }
    else if (registerLanguage == "English")
    {
        translator.load(":/translations/english.qm");
        app.installTranslator(&translator);
    }

    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    QByteArray bytePassword = global::getSettingsValue("passwordDB", "settings").toByteArray();
    QString password = QString(QByteArray::fromBase64(bytePassword));

    db.setHostName(global::getSettingsValue("hostName", "settings").toString());
    db.setDatabaseName(global::getSettingsValue("databaseName", "settings").toString());
    db.setUserName(global::getSettingsValue("userNameDB", "settings").toString());
    db.setPassword(password);
    db.setPort(3306);
    db.open();

    if (!db.isOpen())
    {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setText(QObject::tr("Невозможно подключиться к базам данных!"));
        msgBox.exec();

        SettingsWindow *settingsWindow = new SettingsWindow;
        settingsWindow->show();
        settingsWindow->setAttribute(Qt::WA_DeleteOnClose);

        //singleton
//        SettingsWindow &settingsWindow = SettingsWindow::getInstance();
//        settingsWindow.show();
//        settingsWindow.setAttribute(Qt::WA_DeleteOnClose);
    }

    StartWindow startWindow;
    startWindow.show();

    Notification *notification;
    notification = new NotificationOrder();
    notification = new NotificationSparePart();

    return app.exec();
}
