#include "UpdateOrders.h"
#include "ui_UpdateOrders.h"

UpdateOrders::UpdateOrders(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UpdateOrders)
{
    ui->setupUi(this);

    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowFlags(windowFlags() & Qt::WindowMinimizeButtonHint);
}

UpdateOrders::~UpdateOrders()
{
    delete ui;
}

void UpdateOrders::setValues(const QString& id)
{
    orderId = id;

    QSqlQuery query(ordersDB);

    // add isReady
    query.prepare("SELECT DISTINCT client, date, contacts, auto_model, manufacture_year, VIN_number, discounts, service_address, "
        "auto_license_plate, staff_team, works_list, spare_list, price, feedback "
        "FROM OrdersHistory WHERE id_order = " + orderId);

    query.exec();
    query.next();

    ui->clientLine->setText(query.value(0).toString());
    ui->dateLine->setText(query.value(1).toString());
    ui->contactLine->setText(query.value(2).toString());
    ui->modelLine->setText(query.value(3).toString());
    ui->yearLine->setText(query.value(4).toString());
    ui->VIN_Line->setText(query.value(5).toString());
    //ui->discountsLine->setText(query.value(6).toString());
    //ui->serviceLine->setText(query.value(7).toString());
    ui->autoLicensePlateLine->setText(query.value(8).toString());
    //ui->staffLine->setText(query.value(9).toString());
    //ui->worksLine->setText(query.value(10).toString());
    //ui->sparePartsLine->setText(query.value(11).toString());
    //ui->priceLine->setText(query.value(12).toString());
    //ui->feedbackLine->setText(query.value(13).toString());
}

void UpdateOrders::on_backToViewInfoButton_clicked()
{
    emit sendData(false);

    QDialog::close();
}

void UpdateOrders::on_saveUpdatedInfo_clicked()
{
    QSqlQuery queryOrders(ordersDB);

    QString client = ui->clientLine->text();
    QString date = ui->dateLine->text();
    QString contacts = ui->contactLine->text();
    QString autoModel = ui->modelLine->text();
    QString manufactureYear = ui->yearLine->text();
    QString VIN_Number = ui->VIN_Line->text();
    //QString discounts = ui->discountsLine->text();
    //QString serviceNumber = ui->serviceLine->text();
    QString autoLicensePlate = ui->autoLicensePlateLine->text();
    //QString staffTeam = ui->staffLine->text();
    //QString worksList = ui->worksLine->text();
    //QString spareList = ui->sparePartsLine->text();
    //QString price = ui->priceLine->text();
    //QString feedback = ui->feedbackLine->text();

    queryOrders.prepare("UPDATE OrdersHistory SET client = ?, date = ?, contacts = ?, auto_model = ?, manufacture_year = ?, VIN_number = ?, "
        "discounts = ?, service_address = ?, auto_license_plate = ?, staff_team = ?, works_list = ?, spare_list = ?, price = ?, feedback = ? "
        "WHERE id_order = ?");

    queryOrders.addBindValue(client);
    queryOrders.addBindValue(date);
    queryOrders.addBindValue(contacts);
    queryOrders.addBindValue(autoModel);
    queryOrders.addBindValue(manufactureYear);
    queryOrders.addBindValue(VIN_Number);
    //queryOrders.addBindValue(discounts);
    //queryOrders.addBindValue(serviceNumber);
    queryOrders.addBindValue(autoLicensePlate);
    //queryOrders.addBindValue(staffTeam);
    //queryOrders.addBindValue(worksList);
    //queryOrders.addBindValue(spareList);
    //queryOrders.addBindValue(price);
    //queryOrders.addBindValue(feedback);
    queryOrders.addBindValue(orderId);
    queryOrders.exec();

    QDialog::close();

    QMessageBox::information(this, tr("Уведомление"), tr("Информация о заказе успешно обновлена!"), QMessageBox::Ok);
}
