#include "UpdateOrders.h"
#include "ui_UpdateOrders.h"

UpdateOrders::UpdateOrders(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UpdateOrders)
{
    ui->setupUi(this);

    QDialog::showMaximized();

    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);    
    setWindowFlags(windowFlags() & Qt::WindowMinimizeButtonHint);

    connect(ui->openMapButton, &QAbstractButton::clicked, this, &UpdateOrders::openMap);
//    connect(ui->availableSparePartsTable, &QAbstractItemView::clicked, this, &UpdateOrders::updateUsedSparePartsTable);
//    connect(ui->EmployeesBySetrviceTable, &QAbstractItemView::clicked, this, &UpdateOrders::setOrderEmployees);

    ui->clientErrorLabel->setStyleSheet("color: transparent"); ui->contactsErrorLabel->setStyleSheet("color: transparent");
    ui->autoErrorLabel->setStyleSheet("color: transparent"); ui->serviceErrorLabel->setStyleSheet("color: transparent");
    ui->dateErrorLabel->setStyleSheet("color: transparent");

//    ui->removeLastSparePartButton->setEnabled(false);

    ui->clientTypeComboBox->addItems(QStringList() << tr("Физ. лицо") << tr("Юр. лицо"));
    ui->orderStatusComboBox->addItems(QStringList() << tr("Заявка") << tr("В работе") << tr("Завершен, неоплачен") << tr("Завершен, оплачен"));
    ui->discountsComboBox->addItems(QStringList() << tr("Нет") << tr("Купон") << tr("Акция") << tr("Особые условия") << tr("Постоянный клиент"));
    ui->serviceComboBox->addItems(QStringList() << "Среднефонтанская, 30А (Приморский р-н)" << "Платонова, 56 (Малиновский р-н)" << "Архитекторская, 28 (Киевский р-н)");

//    connect(ui->serviceComboBox, &QComboBox::currentTextChanged, this, &UpdateOrders::updateEmployeesTable);

//    searchFlag = false;

//    setDateAndTime();

//    loadSparePartsTable();
 //   loadEmployeesTable();
}

UpdateOrders::~UpdateOrders()
{
    delete ui;
}

void AddOrder::openMap()
{
    QQmlApplicationEngine *engine = new QQmlApplicationEngine;
    engine->load(QUrl(QStringLiteral("qrc:/mapInteraction.qml")));
}

void UpdateOrders::setValues(const QString &id)
{
    orderId = id;

    QSqlQuery query(ordersDB);

    // add isReady
    query.prepare("SELECT DISTINCT client_type, client, creation_date, creation_time, updating_date, updating_time, contacts, auto_brand, "
                  "auto_model, manufacture_year, VIN_number, auto_license_plate, service_address, discounts, order_status, "
                  "spare_parts_list, works_list, feedback FROM OrdersHistory WHERE id_order = " + orderId);

    query.exec();
    query.next();

    QString clientType = query.value(0).toString();
    if (clientType == "Физ. лицо")
        ui->clientTypeComboBox->setCurrentIndex(0);
    else
        ui->clientTypeComboBox->setCurrentIndex(1);

    ui->clientLine->setText(query.value(1).toString());
    ui->dateLine->setText(query.value(2).toString());
    ui->timeLine->setText(query.value(3).toString());
    ui->updateDateLine->setText(query.value(4).toString());
    ui->updateTimeLine->setText(query.value(5).toString());
    ui->contactLine->setText(query.value(6).toString());
    ui->brandLine->setText(query.value(7).toString());
    ui->modelLine->setText(query.value(8).toString());
    ui->yearLine->setText(query.value(9).toString());
    ui->VIN_Line->setText(query.value(10).toString());
    ui->autoLicensePlateLine->setText(query.value(11).toString());

    QString serviceAddress = query.value(12).toString();
    if (serviceAddress == "Среднефонтанская, 30А (Приморский р-н)")
        ui->serviceComboBox->setCurrentIndex(0);
    else if (serviceAddress == "Платонова, 56 (Малиновский р-н)")
        ui->serviceComboBox->setCurrentIndex(1);
    else if (serviceAddress == "(Архитекторская, 28 (Киевский р-н)")
        ui->serviceComboBox->setCurrentIndex(2);

    QString discountType = query.value(13).toString();
    if (discountType == "Нет" || discountType == "Немає")
        ui->discountsComboBox->setCurrentIndex(0);
    else if (discountType == "Купон" || discountType == "Coupon")
        ui->discountsComboBox->setCurrentIndex(1);
    else if (discountType == "Акция" || discountType == "Акція" || discountType == "Promotion")
        ui->discountsComboBox->setCurrentIndex(2);
    else if (discountType == "Особые условия" || discountType == "Особливі умови")
        ui->discountsComboBox->setCurrentIndex(3);
    else if (discountType == "Постоянный клиент" || discountType == "Постійний клієнт")
        ui->discountsComboBox->setCurrentIndex(4);

    QString orderStatus = query.value(14).toString();
    if (orderStatus == "Заявка")
        ui->orderStatusComboBox->setCurrentIndex(0);
    else if (orderStatus == "В работе")
        ui->orderStatusComboBox->setCurrentIndex(1);
    else if (orderStatus == "Завершен, неоплачен")
        ui->orderStatusComboBox->setCurrentIndex(2);
    else if (orderStatus == "Завершен, оплачен")
        ui->orderStatusComboBox->setCurrentIndex(3);

    ui->sparePartsList->setText(query.value(15).toString());
    ui->worksList->setText(query.value(16).toString());
    ui->feedback->setText(query.value(17).toString());

    QSqlQuery queryEmployee(orderDetailDB);

    queryEmployee.prepare("SELECT order_employee, employee_work_hours, employee_position "
                          "FROM OrderDetailTable WHERE id_order = " + orderId);
    queryEmployee.exec();

    while (queryEmployee.next())
    {
        QString employeePosition = queryEmployee.value(2).toString();

        if (employeePosition == "Механик" && ui->mechanicLine->text().isEmpty())
        {
            ui->mechanicLine->setText(queryEmployee.value(0).toString());
            ui->mechanicHoursLine->setText(queryEmployee.value(1).toString());
        }

        else if (employeePosition == "Механик" && !ui->mechanicLine->text().isEmpty())
        {
            ui->mechanic2Line->setText(queryEmployee.value(0).toString());
            ui->mechanic2HoursLine->setText(queryEmployee.value(1).toString());
        }

        else if (employeePosition == "Диагност")
        {
            ui->diagnosticianLine->setText(queryEmployee.value(0).toString());
            ui->diagnosticianHoursLine->setText(queryEmployee.value(1).toString());
        }

        else if (employeePosition == "Электронщик")
        {
            ui->electronicsLine->setText(queryEmployee.value(0).toString());
            ui->electronicsHoursLine->setText(queryEmployee.value(1).toString());
        }

        else if (employeePosition == "Слесарь")
        {
            ui->locksmithLine->setText(queryEmployee.value(0).toString());
            ui->locksmithHoursLine->setText(queryEmployee.value(1).toString());
        }

        else if (employeePosition == "Мойщик")
        {
            ui->washerLine->setText(queryEmployee.value(0).toString());
            ui->washerHoursLine->setText(queryEmployee.value(1).toString());
        }
    }
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
