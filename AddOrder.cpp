#include "AddOrder.h"
#include "ui_AddOrder.h"

AddOrder::AddOrder(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddOrder)
{
    ui->setupUi(this);

    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowFlags(windowFlags() & Qt::WindowMinimizeButtonHint);

    QDialog::showMaximized();

    QDir tempDirDB = QDir::currentPath(); tempDirDB.cdUp(); QString dirDB = tempDirDB.path();

    ordersHistoryDB = QSqlDatabase::addDatabase("QSQLITE");
    ordersHistoryDB.setDatabaseName(dirDB + "\\CRM_AutoService\\ServiceStationDB.db");
    ordersHistoryDB.open();

    connect(ui->openMapButton, &QAbstractButton::clicked, this, &AddOrder::openMap);
    connect(ui->availableSparePartsTable, &QAbstractItemView::clicked, this, &AddOrder::updateUsedSparePartsTable);
    connect(ui->EmployeesBySetrviceTable, &QAbstractItemView::clicked, this, &AddOrder::setOrderEmployees);

    ui->clientErrorLabel->setStyleSheet("color: transparent"); ui->contactsErrorLabel->setStyleSheet("color: transparent");
    ui->autoErrorLabel->setStyleSheet("color: transparent"); ui->serviceErrorLabel->setStyleSheet("color: transparent");
    ui->dateErrorLabel->setStyleSheet("color: transparent");

    ui->removeLastSparePartButton->setEnabled(false);

    ui->orderStatusComboBox->addItems(QStringList() << tr("Заявка") << tr("В работе") << tr("Завершен, неоплачен") << tr("Завершен, оплачен"));
    ui->discountsComboBox->addItems(QStringList() << tr("Нет") << tr("Купон") << tr("Акция") << tr("Особые условия") << tr("Постоянный клиент"));
    ui->serviceComboBox->addItems(QStringList() << "Среднефонтанская, 30А (Приморский р-н)" << "Платонова, 56 (Малиновский р-н)" << "Архитекторская, 28 (Киевский р-н)");
    ui->clientTypeComboBox->addItems(QStringList() << tr("Физ. лицо") << tr("Юр. лицо"));

    connect(ui->serviceComboBox, &QComboBox::currentTextChanged, this, &AddOrder::updateEmployeesTable);

    searchFlag = false;

    setDateAndTime();

    loadSparePartsTable();
    loadEmployeesTable();
}

AddOrder::~AddOrder()
{
    delete ui;
}

void AddOrder::closeEvent(QCloseEvent *)
{
    QDialog::close();

    listOrders = new ListOrders;
    listOrders->show();
    listOrders->setAttribute(Qt::WA_DeleteOnClose);
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

void AddOrder::loadSparePartsTable()
{
    queryAvailableSparePartsModel = new QSqlQueryModel(this);

    QString queryString = "SELECT id_spare_part, spare_part_name, original, price FROM SparePartsCatalogue ";

    QString searchString;

    if (searchFlag)
        searchString.append("WHERE spare_part_name LIKE '%" + ui->sparePartsSearch->text() + "%' GROUP BY id_spare_part ORDER BY spare_part_name ASC");

    queryString.append(searchString);

    queryAvailableSparePartsModel->setQuery(queryString);

    queryAvailableSparePartsModel->setHeaderData(0, Qt::Horizontal, tr("id"));
    queryAvailableSparePartsModel->setHeaderData(1, Qt::Horizontal, tr("Название"));
    queryAvailableSparePartsModel->insertColumn(2);
    queryAvailableSparePartsModel->setHeaderData(2, Qt::Horizontal, tr("Совместимость"));
    queryAvailableSparePartsModel->setHeaderData(3, Qt::Horizontal, tr("Оригинал"));
    queryAvailableSparePartsModel->setHeaderData(4, Qt::Horizontal, tr("Цена"));

    ui->availableSparePartsTable->setModel(queryAvailableSparePartsModel);

    ui->availableSparePartsTable->setColumnHidden(0, true);

    for (int rowIndex = 0; rowIndex < ui->availableSparePartsTable->model()->rowCount(); ++rowIndex)
        ui->availableSparePartsTable->setIndexWidget(queryAvailableSparePartsModel->index(rowIndex, 2), addWidgetCompatibilityContent(rowIndex));

    ui->availableSparePartsTable->horizontalHeader()->setDefaultSectionSize(maximumWidth());
    ui->availableSparePartsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->availableSparePartsTable->horizontalHeader()->setSectionsClickable(false);
    ui->availableSparePartsTable->resizeColumnsToContents();
    ui->availableSparePartsTable->verticalHeader()->hide();
    ui->availableSparePartsTable->resizeRowsToContents();
}

QWidget* AddOrder::addWidgetCompatibilityContent(int rowIndex)
{
    QWidget *widget = new QWidget(this);
    QHBoxLayout *layout = new QHBoxLayout(widget);
    QLabel *compatibilityContentLabel = new QLabel(widget);

    layout->addWidget(compatibilityContentLabel);

    queryModelLabel = new QSqlQueryModel(this);

    QString queryString = "SELECT REPLACE(auto_compatibility, ', ', '\n') FROM SparePartsCatalogue";

    queryModelLabel->setQuery(queryString, listSparePartsTable);

    QString compatibilityContent = queryModelLabel->data(queryModelLabel->index(rowIndex, 0), Qt::EditRole).toString();

    compatibilityContentLabel->setText(compatibilityContent);
    compatibilityContentLabel->setOpenExternalLinks(true);
    compatibilityContentLabel->setWordWrap(true);

    return widget;
}

void AddOrder::updateUsedSparePartsTable(const QModelIndex &index)
{
    QString sparePartId = queryAvailableSparePartsModel->data(queryAvailableSparePartsModel->index(index.row(), 0), Qt::EditRole).toString();

    QSqlQuery queryCheckAmount(sparePartsDB);

    queryCheckAmount.prepare("SELECT quantity_in_stock FROM SparePartsCatalogue WHERE (id_spare_part = '" + sparePartId + "' AND quantity_in_stock = 0)");
    queryCheckAmount.exec();

    if (queryCheckAmount.first() == 1)
    {
        QMessageBox::warning(this, tr("Предупреждение"), tr("Данная запчасть на складе отсутствует!"), QMessageBox::Ok);
        return;
    }

    QString sparePart = queryAvailableSparePartsModel->data(queryAvailableSparePartsModel->index(index.row(), 1), Qt::EditRole).toString();

    sparePartsList.append(sparePart);
    sparePartsList.append(", ");

    QString sparePartCostDetail = queryAvailableSparePartsModel->data(queryAvailableSparePartsModel->index(index.row(), 4), Qt::EditRole).toString();

    ui->sparePartsList->setText(sparePartsList.replace(", ", (" - " + sparePartCostDetail + "\n")));

    sparePartCost = queryAvailableSparePartsModel->data(queryAvailableSparePartsModel->index(index.row(), 4), Qt::EditRole).toFloat();
    sparePartsCost += sparePartCost;

    sparePartNameLength = sparePart.length() + sparePartCostDetail.length() + 4;

    ui->removeLastSparePartButton->setEnabled(true);
}

void AddOrder::on_removeLastSparePartButton_clicked()
{
   sparePartsList.chop(sparePartNameLength);

   ui->sparePartsList->setText(sparePartsList);

   ui->removeLastSparePartButton->setEnabled(false);
}

void AddOrder::on_clearSparePartsListButton_clicked()
{
    sparePartsList = "";
    ui->sparePartsList->setText("");
    sparePartsCost = 0;

    ui->removeLastSparePartButton->setEnabled(false);
}

void AddOrder::loadEmployeesTable()
{
    queryEmployeesModel = new QSqlQueryModel(this);

    QString queryString = "SELECT id_employee, employee_FML_name, employee_position, hour_payment FROM EmployeesTable WHERE service_address = '" + ui->serviceComboBox->currentText() + "'";

    queryEmployeesModel->setQuery(queryString);

    queryEmployeesModel->setHeaderData(0, Qt::Horizontal, tr("id"));
    queryEmployeesModel->setHeaderData(1, Qt::Horizontal, tr("ФИО сотрудника"));
    queryEmployeesModel->setHeaderData(2, Qt::Horizontal, tr("Должность"));
    queryEmployeesModel->setHeaderData(3, Qt::Horizontal, tr("Почасовая оплата"));

    ui->EmployeesBySetrviceTable->setModel(queryEmployeesModel);

    ui->EmployeesBySetrviceTable->setColumnHidden(0, true);
    ui->EmployeesBySetrviceTable->setColumnHidden(3, true);

    ui->EmployeesBySetrviceTable->horizontalHeader()->setDefaultSectionSize(maximumWidth());
    ui->EmployeesBySetrviceTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->EmployeesBySetrviceTable->horizontalHeader()->setSectionsClickable(false);
    ui->EmployeesBySetrviceTable->resizeColumnsToContents();
    ui->EmployeesBySetrviceTable->verticalHeader()->hide();
}

void AddOrder::setOrderEmployees(const QModelIndex &index)
{
    QString employeeName = queryEmployeesModel->data(queryEmployeesModel->index(index.row(), 1), Qt::EditRole).toString();
    QString employeePosition = queryEmployeesModel->data(queryEmployeesModel->index(index.row(), 2), Qt::EditRole).toString();

    if (ui->mechanicLine->text().isEmpty() && (employeePosition == "Механик" || employeePosition == "Главный механик") &&
            employeeName != ui->mechanic2Line->text())
    {
        ui->mechanicLine->setText(employeeName);
        mechanicHourPayment = queryEmployeesModel->data(queryEmployeesModel->index(index.row(), 3), Qt::EditRole).toInt();
    }

    else if (!ui->mechanicLine->text().isEmpty() && (employeePosition == "Механик" || employeePosition == "Главный механик") &&
             employeeName != ui->mechanicLine->text())
    {
        ui->mechanic2Line->setText(employeeName);
        mechanic2HourPayment = queryEmployeesModel->data(queryEmployeesModel->index(index.row(), 3), Qt::EditRole).toInt();
    }

    else if (employeePosition == "Диагност")
    {
        ui->diagnosticianLine->setText(employeeName);
        diagnosticianHourPayment = queryEmployeesModel->data(queryEmployeesModel->index(index.row(), 3), Qt::EditRole).toInt();
    }

    else if (employeePosition == "Электронщик")
    {
        ui->electronicsLine->setText(employeeName);
        electronicHourPayment = queryEmployeesModel->data(queryEmployeesModel->index(index.row(), 3), Qt::EditRole).toInt();
    }

    else if (employeePosition == "Слесарь")
    {
        ui->locksmithLine->setText(employeeName);
        locksmithHourPayment = queryEmployeesModel->data(queryEmployeesModel->index(index.row(), 3), Qt::EditRole).toInt();
    }

    else if (employeePosition == "Мойщик")
    {
        ui->washerLine->setText(employeeName);
        washerHourPayment = queryEmployeesModel->data(queryEmployeesModel->index(index.row(), 3), Qt::EditRole).toInt();
    }
}

void AddOrder::updateEmployeesTable()
{
    queryEmployeesModel->setQuery(NULL);

    loadEmployeesTable();
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
    QString brandModel = ui->brandLine->text();

    bool error = false;

    if (date.isEmpty())
    {
        error = true; ui->dateErrorLabel->setStyleSheet("color: red");
    }
    else
        ui->dateErrorLabel->setStyleSheet("color: transparent");

    if (client.isEmpty())
    {
        error = true; ui->clientErrorLabel->setStyleSheet("color: red");
    }
    else
        ui->clientErrorLabel->setStyleSheet("color: transparent");

    if (contacts.isEmpty())
    {
        error = true; ui->contactsErrorLabel->setStyleSheet("color: red");
    }
    else
        ui->contactsErrorLabel->setStyleSheet("color: transparent");

    if (brandModel.isEmpty())
    {
        error = true; ui->autoErrorLabel->setStyleSheet("color: red");
    }
    else
        ui->autoErrorLabel->setStyleSheet("color: transparent");

    if (error)
        return;

    queryOrders.prepare("INSERT INTO OrdersHistory (client_type, client, creation_date, creation_time, contacts, auto_brand, auto_model, "
                "manufacture_year, VIN_number, auto_license_plate, service_address, discounts, order_status, spare_parts_list, works_list, feedback) "
                "VALUES(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");

    queryOrders.addBindValue(ui->clientTypeComboBox->currentText());
    queryOrders.addBindValue(client);
    queryOrders.addBindValue(date);
    queryOrders.addBindValue(ui->timeLine->text());
    queryOrders.addBindValue(contacts);
    queryOrders.addBindValue(brandModel);
    queryOrders.addBindValue(ui->modelLine->text());
    queryOrders.addBindValue(ui->yearLine->text());
    queryOrders.addBindValue(ui->VIN_Line->text());
    queryOrders.addBindValue(ui->autoLicensePlateLine->text());
    queryOrders.addBindValue(ui->serviceComboBox->currentText());
    queryOrders.addBindValue(ui->discountsComboBox->currentText());
    queryOrders.addBindValue(ui->orderStatusComboBox->currentText());
    queryOrders.addBindValue(ui->sparePartsList->toPlainText());
    queryOrders.addBindValue(ui->worksList->toPlainText());
    queryOrders.addBindValue(ui->feedback->toPlainText());
    queryOrders.exec();

    int id = queryOrders.lastInsertId().toInt();

    // Simultaneous insertion into detailed order table
    QSqlQuery queryOrderDetail(orderDetailDB);

    if (!ui->mechanicLine->text().isEmpty())
    {
        queryOrderDetail.prepare("INSERT INTO OrderDetailTable (id_order, order_employee, employee_work_hours) VALUES(?, ?, ?)");
        queryOrderDetail.addBindValue(id);
        queryOrderDetail.addBindValue(ui->mechanicLine->text());
        queryOrderDetail.addBindValue(ui->mechanicHoursLine->text());
        queryOrderDetail.exec();
    }

    if (!ui->mechanic2Line->text().isEmpty())
    {
        queryOrderDetail.prepare("INSERT INTO OrderDetailTable (id_order, order_employee, employee_work_hours) VALUES(?, ?, ?)");
        queryOrderDetail.addBindValue(id);
        queryOrderDetail.addBindValue(ui->mechanic2Line->text());
        queryOrderDetail.addBindValue(ui->mechanic2HoursLine->text());
        queryOrderDetail.exec();
    }

    if (!ui->diagnosticianLine->text().isEmpty())
    {
        queryOrderDetail.prepare("INSERT INTO OrderDetailTable (id_order, order_employee, employee_work_hours) VALUES(?, ?, ?)");
        queryOrderDetail.addBindValue(id);
        queryOrderDetail.addBindValue(ui->diagnosticianLine->text());
        queryOrderDetail.addBindValue(ui->diagnosticianHoursLine->text());
        queryOrderDetail.exec();
    }

    if (!ui->electronicsLine->text().isEmpty())
    {
        queryOrderDetail.prepare("INSERT INTO OrderDetailTable (id_order, order_employee, employee_work_hours) VALUES(?, ?, ?)");
        queryOrderDetail.addBindValue(id);
        queryOrderDetail.addBindValue(ui->electronicsLine->text());
        queryOrderDetail.addBindValue(ui->electronicsHoursLine->text());
        queryOrderDetail.exec();
    }

    if (!ui->locksmithLine->text().isEmpty())
    {
        queryOrderDetail.prepare("INSERT INTO OrderDetailTable (id_order, order_employee, employee_work_hours) VALUES(?, ?, ?)");
        queryOrderDetail.addBindValue(id);
        queryOrderDetail.addBindValue(ui->locksmithLine->text());
        queryOrderDetail.addBindValue(ui->locksmithHoursLine->text());
        queryOrderDetail.exec();
    }

    if (!ui->washerLine->text().isEmpty())
    {
        queryOrderDetail.prepare("INSERT INTO OrderDetailTable (id_order, order_employee, employee_work_hours) VALUES(?, ?, ?)");
        queryOrderDetail.addBindValue(id);
        queryOrderDetail.addBindValue(ui->washerLine->text());
        queryOrderDetail.addBindValue(ui->washerHoursLine->text());
        queryOrderDetail.exec();
    }

    // Simultaneous insertion into client table
    QSqlQuery queryClients(clientsDB);

    queryClients.prepare("INSERT INTO ClientsTable (client_type, client_FML_name, contacts, auto_brand, auto_model, auto_license_plate, "
        "manufacture_year, VIN_number) VALUES(?, ?, ?, ?, ?, ?, ?, ?)");

    queryClients.addBindValue(ui->clientTypeComboBox->currentText());
    queryClients.addBindValue(client);
    queryClients.addBindValue(contacts);
    queryClients.addBindValue(brandModel);
    queryClients.addBindValue(ui->modelLine->text());
    queryClients.addBindValue(ui->autoLicensePlateLine->text());
    queryClients.addBindValue(ui->yearLine->text());
    queryClients.addBindValue(ui->VIN_Line->text());
    queryClients.exec();

    // order price calculation
    // employees payment calculation
    float mechanicOverallPayment = 0;
    float mechanic2OverallPayment = 0;
    float diagnosticianOverallPayment = 0;
    float electronicOverallPayment = 0;
    float locksmithOverallPayment = 0;
    float washerOverAllPayment = 0;

    if (!ui->mechanicLine->text().isEmpty() && ui->mechanicHoursLine->text().isEmpty())
        mechanicOverallPayment = mechanicHourPayment * 1;
    else if (!ui->mechanicLine->text().isEmpty() && !ui->mechanicHoursLine->text().isEmpty())
        mechanicOverallPayment = mechanicHourPayment * ui->mechanicHoursLine->text().toFloat();

    if (!ui->mechanic2Line->text().isEmpty() && ui->mechanic2HoursLine->text().isEmpty())
        mechanic2OverallPayment = mechanic2HourPayment * 1;
    else if (!ui->mechanic2Line->text().isEmpty() && !ui->mechanic2HoursLine->text().isEmpty())
        mechanic2OverallPayment = mechanic2HourPayment * ui->mechanic2HoursLine->text().toFloat();

    if (!ui->diagnosticianLine->text().isEmpty() && ui->diagnosticianHoursLine->text().isEmpty())
        diagnosticianOverallPayment = diagnosticianHourPayment * 1;
    else if (!ui->diagnosticianLine->text().isEmpty() && !ui->diagnosticianHoursLine->text().isEmpty())
        diagnosticianOverallPayment = diagnosticianHourPayment * ui->diagnosticianHoursLine->text().toFloat();

    if (!ui->electronicsLine->text().isEmpty() && ui->electronicsHoursLine->text().isEmpty())
        electronicOverallPayment = electronicHourPayment * 1;
    else if (!ui->electronicsLine->text().isEmpty() && !ui->electronicsHoursLine->text().isEmpty())
        electronicOverallPayment = electronicHourPayment * ui->electronicsHoursLine->text().toFloat();

    if (!ui->locksmithLine->text().isEmpty() && ui->locksmithHoursLine->text().isEmpty())
        locksmithOverallPayment = locksmithHourPayment * 1;
    else if (!ui->locksmithLine->text().isEmpty() && !ui->locksmithHoursLine->text().isEmpty())
        locksmithOverallPayment = locksmithHourPayment * ui->locksmithHoursLine->text().toFloat();

    if (!ui->washerLine->text().isEmpty() && ui->washerHoursLine->text().isEmpty())
        washerOverAllPayment = washerHourPayment * 1;
    else if (!ui->washerLine->text().isEmpty() && !ui->washerHoursLine->text().isEmpty())
        washerOverAllPayment = washerHourPayment * ui->washerHoursLine->text().toFloat();

    float orderTotalCost = sparePartsCost + mechanicOverallPayment + mechanic2OverallPayment + diagnosticianOverallPayment
                         + electronicOverallPayment + locksmithOverallPayment + washerOverAllPayment;

    // disounts calculation
    float orderTotalDiscountCost = 0;

    if (ui->discountsComboBox->currentIndex() == 0)
        orderTotalDiscountCost = orderTotalCost;

    else if (ui->discountsComboBox->currentIndex() == 1)
        orderTotalDiscountCost = orderTotalCost - ((orderTotalCost * couponDiscount) / 100);

    else if (ui->discountsComboBox->currentIndex() == 2)
        orderTotalDiscountCost = orderTotalCost - ((orderTotalCost * promotionDiscount) / 100);

    else if (ui->discountsComboBox->currentIndex() == 3)
        orderTotalDiscountCost = orderTotalCost - ((orderTotalCost * specialCondidtionsDiscount) / 100);

    else if (ui->discountsComboBox->currentIndex() == 4)
        orderTotalDiscountCost = orderTotalCost - ((orderTotalCost * regularCustomerDiscount) / 100);

    // cost update
    QString orderTotalDiscountCostDB = QString("%1").arg(orderTotalDiscountCost, 0, 'f', 2);

    queryOrders.prepare("UPDATE OrdersHistory SET price = ? WHERE id_order = ?");
    queryOrders.addBindValue(orderTotalDiscountCostDB);
    queryOrders.addBindValue(id);
    queryOrders.exec();

    QDialog::close();

    QMessageBox::information(this, tr("Уведомление"), tr("Заказ успешно создан!"), QMessageBox::Ok);
}

void AddOrder::on_sparePartsSearch_returnPressed()
{
    searchFlag = true;

    updateSparePartsTable();
}

void AddOrder::updateSparePartsTable()
{
    queryAvailableSparePartsModel->setQuery(NULL);

    loadSparePartsTable();
}

void AddOrder::on_clearMechanicButton_clicked()
{
    ui->mechanicLine->setText("");
    ui->mechanicHoursLine->setText("");
}

void AddOrder::on_clearMechanic2Button_clicked()
{
    ui->mechanic2Line->setText("");
    ui->mechanic2HoursLine->setText("");
}

void AddOrder::on_clearDiagnosticianButton_clicked()
{
    ui->diagnosticianLine->setText("");
    ui->diagnosticianHoursLine->setText("");
}

void AddOrder::on_clearElectronicButton_clicked()
{
    ui->electronicsLine->setText("");
    ui->electronicsHoursLine->setText("");
}

void AddOrder::on_clearLocksmithButton_clicked()
{
    ui->locksmithLine->setText("");
    ui->locksmithHoursLine->setText("");
}

void AddOrder::on_clearWasherButton_clicked()
{
    ui->washerLine->setText("");
    ui->washerHoursLine->setText("");
}
