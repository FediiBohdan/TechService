#include "AddOrder.h"
#include "StartWindow.h"
#include "Global.h"

#include <QApplication>
#include <QTranslator>
#include <QMessageBox>
#include <QLockFile>
#include <QObject>
#include <QDebug>
#include <QDir>

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

    StartWindow startWindow;
    startWindow.show();

    return app.exec();
}

