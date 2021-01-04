#include "AddOrder.h"
#include "StartWindow.h"
#include "Global.h"

#include <QApplication>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    app.setApplicationName("TechService CRM");
    app.setOrganizationName("TechService");
    app.setApplicationVersion("1.0.0.0");

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

