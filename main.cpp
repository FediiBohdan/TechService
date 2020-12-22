#include <QApplication>
#include <OrderCreation.h>
#include <StartWindow.h>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    StartWindow startWindow;
    startWindow.show();

    return app.exec();
}

