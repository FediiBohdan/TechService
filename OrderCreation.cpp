#include "OrderCreation.h"
#include "ui_OrderCreation.h"

OrderCreation::OrderCreation(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OrderCreation)
{
    ui->setupUi(this);

    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowFlags(windowFlags() & Qt::WindowMinimizeButtonHint);

    connect(ui->pushButton, &QAbstractButton::clicked, this, &OrderCreation::openMap);

    createOrder();
}

OrderCreation::~OrderCreation()
{
    delete ui;
}

void OrderCreation::openMap()
{
    QQmlApplicationEngine *engine = new QQmlApplicationEngine;
    engine->load(QUrl(QStringLiteral("qrc:/mapInteraction.qml")));
}

void OrderCreation::createOrder()
{
    QDate currentDate = QDate::currentDate();
    QTime currentTime = QTime::currentTime();
    ui->dateLine->setText(currentDate.toString(Qt::SystemLocaleDate));
    ui->timeLine->setText(currentTime.toString(Qt::SystemLocaleDate));
}

void OrderCreation::closeWindow()
{
    close();
}

