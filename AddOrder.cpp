#include "AddOrder.h"
#include "ui_AddOrder.h"

AddOrder::AddOrder(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddOrder)
{
    ui->setupUi(this);

    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowFlags(windowFlags() & Qt::WindowMinimizeButtonHint);

    connect(ui->pushButton, &QAbstractButton::clicked, this, &AddOrder::openMap);

    QDir tempDirDB = QDir::currentPath(); tempDirDB.cdUp(); QString dirDB = tempDirDB.path();

    ordersHistoryDB = QSqlDatabase::addDatabase("QSQLITE");
    ordersHistoryDB.setDatabaseName(dirDB + "\\CRM_AutoService\\ServiceStationDB.db");
    ordersHistoryDB.open();

    setDateAndTime();
}

AddOrder::~AddOrder()
{
    delete ui;
}

void AddOrder::openMap()
{
    QQmlApplicationEngine *engine = new QQmlApplicationEngine;
    engine->load(QUrl(QStringLiteral("qrc:/mapInteraction.qml")));
}

void AddOrder::closeWindow()
{
    close();
}

void AddOrder::setDateAndTime()
{
    QDate currentDate = QDate::currentDate();
    QTime currentTime = QTime::currentTime();
    ui->dateLine->setText(currentDate.toString(Qt::SystemLocaleDate));
    ui->timeLine->setText(currentTime.toString(Qt::SystemLocaleDate));
}

void AddOrder::on_createOrderButton_clicked()
{
    // Insertion into order table
    QSqlQuery queryOrders(ordersHistoryDB);

    QString client = ui->clientLine->text();
    QString date = ui->dateLine->text();
    QString contacts = ui->contactLine->text();
    QString autoModel = ui->modelLine->text();
    QString manufactureYear = ui->yearLine->text();
    QString VIN_Number = ui->VIN_Line->text();
    QString discounts = ui->discountsLine->text();
    QString serviceNumber = ui->serviceLine->text();
    QString autoLicensePlate = ui->autoLicensePlateLine->text();
    QString staffTeam = ui->staffLine->text();
    QString worksList = ui->worksLine->text();
    QString spareList = ui->sparePartsLine->text();
    QString price = ui->priceLine->text();
    QString feedback = ui->feedbackLine->text();

    queryOrders.prepare("INSERT INTO OrdersHistory (client, date, contacts, auto_model, manufacture_year, VIN_number, "
        "discounts, service_number, auto_license_plate, staff_team, works_list, spare_list, price, feedback) "
        "VALUES(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");

    queryOrders.addBindValue(client);
    queryOrders.addBindValue(date);
    queryOrders.addBindValue(contacts);
    queryOrders.addBindValue(autoModel);
    queryOrders.addBindValue(manufactureYear);
    queryOrders.addBindValue(VIN_Number);
    queryOrders.addBindValue(discounts);
    queryOrders.addBindValue(serviceNumber);
    queryOrders.addBindValue(autoLicensePlate);
    queryOrders.addBindValue(staffTeam);
    queryOrders.addBindValue(worksList);
    queryOrders.addBindValue(spareList);
    queryOrders.addBindValue(price);
    queryOrders.addBindValue(feedback);
    queryOrders.exec();

    // Simultaneous insertion into client table
    QSqlQuery queryClients(clientsDB);

    QString clientClientsDB = ui->clientLine->text();
    QString contactsClientsDB = ui->contactLine->text();
    QString autoModelClientsDB = ui->modelLine->text();
    QString autoLicensePlateClientsDB = ui->autoLicensePlateLine->text();
    QString manufactureYearClientsDB = ui->yearLine->text();
    QString VIN_NumberClientsDB = ui->VIN_Line->text();

    queryClients.prepare("INSERT INTO ClientsTable (client_FML_name, contacts, auto_model, auto_license_plate, "
        "manufacture_year, VIN_number) "
        "VALUES(?, ?, ?, ?, ?, ?)");

    queryClients.addBindValue(clientClientsDB);
    queryClients.addBindValue(contactsClientsDB);
    queryClients.addBindValue(autoModelClientsDB);
    queryClients.addBindValue(autoLicensePlateClientsDB);
    queryClients.addBindValue(manufactureYearClientsDB);
    queryClients.addBindValue(VIN_NumberClientsDB);
    queryClients.exec();

    QDialog::close();

    QMessageBox::information(this, tr("Уведомление"), tr("Заказ успешно создан!"), QMessageBox::Ok);
}
