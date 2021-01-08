#include "ViewOrders.h"
#include "ui_ViewOrders.h"

ViewOrders::ViewOrders(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ViewOrders)
{
    ui->setupUi(this);

    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowFlags(windowFlags() & Qt::WindowMinimizeButtonHint);
}

ViewOrders::~ViewOrders()
{
    delete ui;
}

void ViewOrders::closeEvent(QCloseEvent*)
{
    QDialog::hide();

    listOrders = new ListOrders;
    listOrders->show();
    listOrders->setAttribute(Qt::WA_DeleteOnClose);
}

// Gets spare part's id from ListSpareParts
void ViewOrders::setValues(const QString &id)
{
    orderId = id;

    QSqlQuery query(ordersDB);

    // add isReady
    query.prepare("SELECT DISTINCT client, date, contacts, auto_model, manufacture_year, VIN_number, discounts, service_number, "
                  "auto_license_plate, staff_team, works_list, spare_list, price, feedback "
                  "FROM OrdersHistory WHERE id_order = " + orderId);
    qDebug()<<query.exec();
    query.next();

    ui->clientLine->setText(query.value(0).toString());
    ui->dateLine->setText(query.value(1).toString());
    ui->contactLine->setText(query.value(2).toString());
    ui->modelLine->setText(query.value(3).toString());
    ui->yearLine->setText(query.value(4).toString());
    ui->VIN_Line->setText(query.value(5).toString());
    ui->discountsLine->setText(query.value(6).toString());
    ui->serviceLine->setText(query.value(7).toString());
    ui->autoLicensePlateLine->setText(query.value(8).toString());
    ui->staffLine->setText(query.value(9).toString());
    ui->worksLine->setText(query.value(10).toString());
    ui->sparePartsLine->setText(query.value(11).toString());
    ui->priceLine->setText(query.value(12).toString());
    ui->feedbackLine->setText(query.value(13).toString());
}
