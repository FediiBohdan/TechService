#include <QApplication>
#include <OrderCreation.h>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    OrderCreation orderCreation;
    orderCreation.show();

    return app.exec();
}

