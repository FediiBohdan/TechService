#include "OrderCreation.h"
#include "ui_OrderCreation.h"

OrderCreation::OrderCreation(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OrderCreation)
{
    ui->setupUi(this);

    connect(ui->pushButton, &QAbstractButton::clicked, this, &OrderCreation::openMap);
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


void OrderCreation::on_openStaffTable_clicked()
{
    staffTable = new StaffTable;
    staffTable->show();
    staffTable->setAttribute(Qt::WA_DeleteOnClose);
}
