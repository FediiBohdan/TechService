#include <QApplication>
#include <OrderCreation.h>
#include <StartWindow.h>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    app.setApplicationName("CRM AutoService");
    app.setOrganizationName("TechService");
    app.setApplicationVersion("1.0.0.0");

    StartWindow startWindow;
    startWindow.show();

    return app.exec();
}

