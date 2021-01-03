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

    ordersHistoryDB = QSqlDatabase::addDatabase("QSQLITE");
    ordersHistoryDB.setDatabaseName("D:\\Diploma\\CRM_AutoService\\ServiceStationDB.db");
    ordersHistoryDB.open();

    setDateAndTime();
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

void OrderCreation::closeWindow()
{
    close();
}

void OrderCreation::setDateAndTime()
{
    QDate currentDate = QDate::currentDate();
    QTime currentTime = QTime::currentTime();
    ui->dateLine->setText(currentDate.toString(Qt::SystemLocaleDate));
    ui->timeLine->setText(currentTime.toString(Qt::SystemLocaleDate));
}

void OrderCreation::on_createOrderButton_clicked()
{
    QSqlQuery query(ordersHistoryDB);

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

    qDebug() << __LINE__ << query.prepare("INSERT INTO OrdersHistory (client, date, contacts, auto_model, manufacture_year, VIN_number, discounts, service_number, auto_license_plate, staff_team, works_list, spare_list, price, feedback)"
                  "VALUES(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");

    query.addBindValue(client);
    query.addBindValue(date);
    query.addBindValue(contacts);
    query.addBindValue(autoModel);
    query.addBindValue(manufactureYear);
    query.addBindValue(VIN_Number);
    query.addBindValue(discounts);
    query.addBindValue(serviceNumber);
    query.addBindValue(autoLicensePlate);
    query.addBindValue(staffTeam);
    query.addBindValue(worksList);
    query.addBindValue(spareList);
    query.addBindValue(price);
    query.addBindValue(feedback);
    qDebug() << __LINE__ << query.exec();

    close();

    QMessageBox::information(this, tr("Уведомление"), tr("Заказ успешно создан!"), QMessageBox::Ok);
}
