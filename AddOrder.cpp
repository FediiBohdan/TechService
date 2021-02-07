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

    ui->clientErrorLabel->setStyleSheet("color: transparent"); ui->contactsErrorLabel->setStyleSheet("color: transparent");
    ui->autoErrorLabel->setStyleSheet("color: transparent"); ui->serviceErrorLabel->setStyleSheet("color: transparent");
    ui->dateErrorLabel->setStyleSheet("color: transparent");

    ui->orderStatusComboBox->addItems(QStringList() << tr("Заявка") << tr("В работе") << tr("Завершен, неоплачен") << tr("Завершен, оплачен"));
    ui->discountsComboBox->addItems(QStringList() << tr("Нет") << tr("Купон") << tr("Акция") << tr("Особые условия") << tr("Постоянный клиент"));
    ui->serviceComboBox->addItems(QStringList() << "Street A, 123" << "Street B, 456" << "Street C, 789");
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
//    QDialog::close();

//    listOrders = new ListOrders;
//    listOrders->show();
//    listOrders->setAttribute(Qt::WA_DeleteOnClose);
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

    sparePartsList.append(queryAvailableSparePartsModel->data(queryAvailableSparePartsModel->index(index.row(), 1), Qt::EditRole).toString());
    sparePartsList.append(", ");

    QString sparePartCostDetail = queryAvailableSparePartsModel->data(queryAvailableSparePartsModel->index(index.row(), 4), Qt::EditRole).toString();

    ui->sparePartsList->setText(sparePartsList.replace(", ", (" - " + sparePartCostDetail + "\n")));

    sparePartCost = queryAvailableSparePartsModel->data(queryAvailableSparePartsModel->index(index.row(), 4), Qt::EditRole).toInt();
    sparePartsCost += sparePartCost;
}

void AddOrder::on_clearSparePartsListButton_clicked()
{
    sparePartsList = "";
    ui->sparePartsList->setText("");
    sparePartsCost = 0;
}

void AddOrder::loadEmployeesTable()
{
    queryEmployeesModel = new QSqlQueryModel(this);

    QString queryString = "SELECT id_employee, employee_FML_name, employee_position FROM EmployeesTable WHERE service_address = '" + ui->serviceComboBox->currentText() + "'";

    queryEmployeesModel->setQuery(queryString);

    queryEmployeesModel->setHeaderData(0, Qt::Horizontal, tr("id"));
    queryEmployeesModel->setHeaderData(1, Qt::Horizontal, tr("ФИО сотрудника"));
    queryEmployeesModel->setHeaderData(2, Qt::Horizontal, tr("Должность"));
    queryEmployeesModel->insertColumn(3);
    queryEmployeesModel->setHeaderData(3, Qt::Horizontal, tr("Часы"));

    ui->allEmployees->setModel(queryEmployeesModel);

    ui->allEmployees->setColumnHidden(0, true);

    for (int rowIndex = 0; rowIndex < ui->allEmployees->model()->rowCount(); ++rowIndex)
        ui->allEmployees->setIndexWidget(queryEmployeesModel->index(rowIndex, 3), addWidgetHoursLine(rowIndex));

    ui->allEmployees->horizontalHeader()->setDefaultSectionSize(maximumWidth());
    ui->allEmployees->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->allEmployees->horizontalHeader()->setSectionsClickable(false);
    ui->allEmployees->resizeColumnsToContents();
    ui->allEmployees->verticalHeader()->hide();
    ui->allEmployees->resizeRowsToContents();
}

QWidget* AddOrder::addWidgetHoursLine(int rowIndex)
{
    QWidget *widget = new QWidget(this);
    QHBoxLayout *layout = new QHBoxLayout(widget);
    QLineEdit *hoursLineEdit = new QLineEdit(widget);

    layout->addWidget(hoursLineEdit);

    queryEmployeesHoursModel = new QSqlQueryModel(this);

    QString queryString = "SELECT hour_payment FROM EmployeesTable";

    queryEmployeesHoursModel->setQuery(queryString, listSparePartsTable);

    QString hours = queryEmployeesHoursModel->data(queryEmployeesHoursModel->index(rowIndex, 0), Qt::EditRole).toString();

    hoursLineEdit->setText(hours);

    return widget;
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
    QString autoModel = ui->modelLine->text();
    QString manufactureYear = ui->yearLine->text();
    QString VIN_Number = ui->VIN_Line->text();
    QString discounts = ui->discountsComboBox->currentText();
    QString serviceNumber = ui->serviceComboBox->currentText();
    QString autoLicensePlate = ui->autoLicensePlateLine->text();
    //QString staffTeam = ui->staffLine->text();
    QString worksList = ui->worksList->toPlainText();
    QString feedback = ui->feedback->toPlainText();

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

    if (autoModel.isEmpty())
    {
        error = true; ui->autoErrorLabel->setStyleSheet("color: red");
    }
    else
        ui->autoErrorLabel->setStyleSheet("color: transparent");

//    if (serviceNumber.isEmpty())
//    {
//        error = true; ui->serviceErrorLabel->setStyleSheet("color: red");
//    }
//    else
//        ui->serviceErrorLabel->setStyleSheet("color: transparent");

    if (error)
        return;

    queryOrders.prepare("INSERT INTO OrdersHistory (client, creation_date, contacts, auto_model, manufacture_year, VIN_number, "
        "discounts, service_address, auto_license_plate, works_list, spare_parts_list, feedback) "
        "VALUES(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");

    queryOrders.addBindValue(client);
    queryOrders.addBindValue(date);
    queryOrders.addBindValue(contacts);
    queryOrders.addBindValue(autoModel);
    queryOrders.addBindValue(manufactureYear);
    queryOrders.addBindValue(VIN_Number);
    queryOrders.addBindValue(discounts);
    queryOrders.addBindValue(serviceNumber);
    queryOrders.addBindValue(autoLicensePlate);
    //queryOrders.addBindValue(staffTeam);
    queryOrders.addBindValue(worksList);
    queryOrders.addBindValue(sparePartsList);
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
