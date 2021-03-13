#include "ViewUpdateOrder.h"
#include "ui_ViewUpdateOrder.h"

ViewUpdateOrder::ViewUpdateOrder(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ViewUpdateOrder)
{
    ui->setupUi(this);

    QDialog::showMaximized();

    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);    
    setWindowFlags(windowFlags() & Qt::WindowMinimizeButtonHint);

    connect(ui->availableSparePartsTable, &QAbstractItemView::clicked, this, &ViewUpdateOrder::updateUsedSparePartsListTable);
    connect(ui->usedSparePartsTableView, &QAbstractItemView::clicked, this, &ViewUpdateOrder::removeUsedSparePartsTable);
    connect(ui->employeesByServiceTable, &QAbstractItemView::clicked, this, &ViewUpdateOrder::setOrderEmployees);

    ui->clientErrorLabel->setStyleSheet("color: transparent"); ui->contactsErrorLabel->setStyleSheet("color: transparent");
    ui->autoErrorLabel->setStyleSheet("color: transparent"); ui->serviceErrorLabel->setStyleSheet("color: transparent");
    ui->dateErrorLabel->setStyleSheet("color: transparent");

    ui->clientTypeComboBox->addItems(QStringList() << tr("Физ. лицо") << tr("Юр. лицо"));
    ui->orderStatusComboBox->addItems(QStringList() << tr("Заявка") << tr("В работе") << tr("Завершен, неоплачен") << tr("Завершен, оплачен"));
    ui->discountsComboBox->addItems(QStringList() << tr("Нет") << tr("Купон") << tr("Акция") << tr("Особые условия") << tr("Постоянный клиент"));
    ui->serviceComboBox->addItems(QStringList() << "Среднефонтанская, 30А (Приморский р-н)" << "Платонова, 56 (Малиновский р-н)" << "Архитекторская, 28 (Киевский р-н)");

    connect(ui->serviceComboBox, &QComboBox::currentTextChanged, this, &ViewUpdateOrder::updateEmployeesTable);

    //QSqlQuery queryRemoveEmployeeModel(employeesDB);

    searchFlag = false;

    setDateAndTime();

    loadSparePartsTable();
    loadEmployeesTable();
}

ViewUpdateOrder::~ViewUpdateOrder()
{
    delete ui;
}

void ViewUpdateOrder::closeEvent(QCloseEvent *)
{
    QDialog::close();

    listOrders = new ListOrders;
    listOrders->show();
    listOrders->setAttribute(Qt::WA_DeleteOnClose);
}

void ViewUpdateOrder::loadSparePartsTable()
{
    queryAvailableSparePartsModel = new QSqlQueryModel(this);

    QString queryString = "SELECT id_spare_part, spare_part_name, quantity_in_stock, auto_compatibility, original, price FROM SparePartsCatalogue ";

    QString searchString;

    if (searchFlag)
        searchString.append("WHERE spare_part_name LIKE '%" + ui->sparePartsSearch->text() + "%' GROUP BY id_spare_part ORDER BY spare_part_name ASC");

    queryString.append(searchString);

    queryAvailableSparePartsModel->setQuery(queryString);

    queryAvailableSparePartsModel->setHeaderData(0, Qt::Horizontal, tr("id"));
    queryAvailableSparePartsModel->setHeaderData(1, Qt::Horizontal, tr("Название"));
    queryAvailableSparePartsModel->setHeaderData(2, Qt::Horizontal, tr("Количество"));
    queryAvailableSparePartsModel->setHeaderData(3, Qt::Horizontal, tr("Совместимость"));
    queryAvailableSparePartsModel->setHeaderData(4, Qt::Horizontal, tr("Оригинал"));
    queryAvailableSparePartsModel->setHeaderData(5, Qt::Horizontal, tr("Цена"));

    ui->availableSparePartsTable->setModel(queryAvailableSparePartsModel);

    ui->availableSparePartsTable->setColumnHidden(0, true);
    ui->availableSparePartsTable->setColumnHidden(2, true);

    ui->availableSparePartsTable->horizontalHeader()->setDefaultSectionSize(maximumWidth());
    ui->availableSparePartsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->availableSparePartsTable->horizontalHeader()->setSectionsClickable(false);
    ui->availableSparePartsTable->resizeColumnsToContents();
    ui->availableSparePartsTable->verticalHeader()->hide();
    ui->availableSparePartsTable->resizeRowsToContents();
}

// shows table with used spare parts
void ViewUpdateOrder::loadUsedSparePartsTable()
{
    queryGetUsedSparePartsModel = new QSqlQueryModel(this);

    QString queryString = "SELECT id_order_spare_part, id_order, id_spare_part, order_spare_part, order_spare_part_price FROM OrderSpareParts "
                          "WHERE id_order = '" + orderId + "'";

    queryGetUsedSparePartsModel->setQuery(queryString);

    queryGetUsedSparePartsModel->setHeaderData(0, Qt::Horizontal, tr("id_order_spare_part"));
    queryGetUsedSparePartsModel->setHeaderData(1, Qt::Horizontal, tr("id_order"));
    queryGetUsedSparePartsModel->setHeaderData(2, Qt::Horizontal, tr("id_spare_part"));
    queryGetUsedSparePartsModel->setHeaderData(3, Qt::Horizontal, tr("Название"));
    queryGetUsedSparePartsModel->setHeaderData(4, Qt::Horizontal, tr("Цена"));

    ui->usedSparePartsTableView->setModel(queryGetUsedSparePartsModel);

    ui->usedSparePartsTableView->setColumnHidden(0, true);
    ui->usedSparePartsTableView->setColumnHidden(1, true);
    ui->usedSparePartsTableView->setColumnHidden(2, true);

    ui->usedSparePartsTableView->resizeRowsToContents();
    ui->usedSparePartsTableView->verticalHeader()->hide();
    ui->usedSparePartsTableView->resizeColumnsToContents();
    ui->usedSparePartsTableView->horizontalHeader()->setSectionsClickable(false);
    ui->usedSparePartsTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->usedSparePartsTableView->horizontalHeader()->setDefaultSectionSize(maximumWidth());
}

// choosing needed spare parts for the order + amount check
void ViewUpdateOrder::updateUsedSparePartsListTable(const QModelIndex &index)
{
    QSqlQuery queryUsedSparePartsModel(sparePartsDB);

    QString sparePartId = queryAvailableSparePartsModel->data(queryAvailableSparePartsModel->index(index.row(), 0), Qt::EditRole).toString();
    QString sparePartName = queryAvailableSparePartsModel->data(queryAvailableSparePartsModel->index(index.row(), 1), Qt::EditRole).toString();
    QString sparePartPrice = queryAvailableSparePartsModel->data(queryAvailableSparePartsModel->index(index.row(), 5), Qt::EditRole).toString();

    QSqlQuery queryCheckAmount(sparePartsDB);

    queryCheckAmount.prepare("SELECT quantity_in_stock FROM SparePartsCatalogue WHERE (id_spare_part = '" + sparePartId + "' AND quantity_in_stock = 0)");
    queryCheckAmount.exec();

    if (queryCheckAmount.first() == 1)
    {
        QMessageBox::warning(this, tr("Предупреждение"), tr("Данная запчасть на складе отсутствует!"), QMessageBox::Ok);
        return;
    }

    // insertion spare parts by order to OrderSpareParts table
    queryUsedSparePartsModel.prepare("INSERT INTO OrderSpareParts (id_order, id_spare_part, order_spare_part, order_spare_part_price) VALUES(?, ?, ?, ?)");
    queryUsedSparePartsModel.addBindValue(orderId);
    queryUsedSparePartsModel.addBindValue(sparePartId);
    queryUsedSparePartsModel.addBindValue(sparePartName);
    queryUsedSparePartsModel.addBindValue(sparePartPrice);
    queryUsedSparePartsModel.exec();

    // spare part is removed from available spare parts list
    int sparePartsAmount = queryAvailableSparePartsModel->data(queryAvailableSparePartsModel->index(index.row(), 2), Qt::EditRole).toInt();

    QSqlQuery querySpareParts(sparePartsDB);

    querySpareParts.prepare("UPDATE SparePartsCatalogue SET quantity_in_stock = ? WHERE id_spare_part = ?");
    querySpareParts.addBindValue(sparePartsAmount - 1);
    querySpareParts.addBindValue(sparePartId);
    querySpareParts.exec();

    loadUsedSparePartsTable();
    updateAvailableSparePartsTable();
}

// removing spare parts from used spare parts table
void ViewUpdateOrder::removeUsedSparePartsTable(const QModelIndex &index)
{
    QString orderSparePartId = queryGetUsedSparePartsModel->data(queryGetUsedSparePartsModel->index(index.row(), 0), Qt::EditRole).toString();
    QString returnSparePartId = queryGetUsedSparePartsModel->data(queryGetUsedSparePartsModel->index(index.row(), 2), Qt::EditRole).toString();

    QSqlQuery queryRemoveUsedSparePartsModel(sparePartsDB);

    queryRemoveUsedSparePartsModel.prepare("DELETE FROM OrderSpareParts WHERE id_order_spare_part = ?");
    queryRemoveUsedSparePartsModel.addBindValue(orderSparePartId);
    queryRemoveUsedSparePartsModel.exec();

    // spare part is returned to available spare parts list
    // firstly check the amount of returning spare part
    QSqlQuery query(sparePartsDB);

    query.prepare("SELECT quantity_in_stock FROM SparePartsCatalogue WHERE id_spare_part = " + returnSparePartId);
    query.exec();
    query.next();

    int sparePartsAmount = query.value(0).toInt();

    // then increase its amount in available spare parts
    QSqlQuery querySpareParts(sparePartsDB);

    querySpareParts.prepare("UPDATE SparePartsCatalogue SET quantity_in_stock = ? WHERE id_spare_part = ?");
    querySpareParts.addBindValue(sparePartsAmount + 1);
    querySpareParts.addBindValue(returnSparePartId);
    querySpareParts.exec();

    updateUsedSparePartsTable();
    updateAvailableSparePartsTable();
}

void ViewUpdateOrder::updateUsedSparePartsTable()
{
    queryGetUsedSparePartsModel->setQuery(NULL);

    loadUsedSparePartsTable();
}

void ViewUpdateOrder::updateAvailableSparePartsTable()
{
    queryAvailableSparePartsModel->setQuery(NULL);
    loadSparePartsTable();
}

void ViewUpdateOrder::loadEmployeesTable()
{
    queryEmployeesModel = new QSqlQueryModel(this);

    QString queryString = "SELECT id_employee, employee_FML_name, employee_position, hour_payment FROM EmployeesTable WHERE service_address = '" + ui->serviceComboBox->currentText() + "'";

    queryEmployeesModel->setQuery(queryString);

    queryEmployeesModel->setHeaderData(0, Qt::Horizontal, tr("id"));
    queryEmployeesModel->setHeaderData(1, Qt::Horizontal, tr("ФИО сотрудника"));
    queryEmployeesModel->setHeaderData(2, Qt::Horizontal, tr("Должность"));
    queryEmployeesModel->setHeaderData(3, Qt::Horizontal, tr("Почасовая оплата"));

    ui->employeesByServiceTable->setModel(queryEmployeesModel);

    ui->employeesByServiceTable->setColumnHidden(0, true);
    ui->employeesByServiceTable->setColumnHidden(3, true);

    ui->employeesByServiceTable->horizontalHeader()->setDefaultSectionSize(maximumWidth());
    ui->employeesByServiceTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->employeesByServiceTable->horizontalHeader()->setSectionsClickable(false);
    ui->employeesByServiceTable->resizeColumnsToContents();
    ui->employeesByServiceTable->verticalHeader()->hide();
}

void ViewUpdateOrder::setOrderEmployees(const QModelIndex &index)
{
    QString employeeName = queryEmployeesModel->data(queryEmployeesModel->index(index.row(), 1), Qt::EditRole).toString();
    QString employeePosition = queryEmployeesModel->data(queryEmployeesModel->index(index.row(), 2), Qt::EditRole).toString();
    int employeeId = queryEmployeesModel->data(queryEmployeesModel->index(index.row(), 0), Qt::EditRole).toInt();

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

    // Simultaneous insertion into detailed order table
    QSqlQuery queryOrderDetail(orderDetailDB);
    QSqlQuery queryCheckEmployee(orderDetailDB);
    queryCheckEmployee.prepare("SELECT EXISTS (SELECT id_employee FROM OrderDetailTable WHERE id_order = '" + orderId + "' AND id_employee = '" + employeeId + "')");
    queryCheckEmployee.exec();
    queryCheckEmployee.next();

    if (queryCheckEmployee.value(0) != 0) // if employee exists, just update him
    {
        if (!ui->mechanicLine->text().isEmpty() && mechanicFlag == false) //mechanic
        {
            queryOrderDetail.prepare("UPDATE OrderDetailTable SET order_employee = ?, employee_work_hours = ?, employee_position = ? WHERE id_order = ? AND id_employee = ?");
            queryOrderDetail.addBindValue(ui->mechanicLine->text());
            queryOrderDetail.addBindValue(ui->mechanicHoursLine->text());
            queryOrderDetail.addBindValue("Механик");
            queryOrderDetail.addBindValue(orderId);
            queryOrderDetail.addBindValue(employeeId);
            queryOrderDetail.exec();
        }
        else if (ui->mechanicLine->text().isEmpty() && mechanicFlag == false)
        {
            queryOrderDetail.prepare("UPDATE OrderDetailTable SET order_employee = NULL, employee_work_hours = NULL, employee_position = NULL WHERE id_order = ? AND id_employee = ?");
            queryOrderDetail.addBindValue(orderId);
            queryOrderDetail.addBindValue(employeeId);
            queryOrderDetail.exec();
        }

        if (!ui->mechanic2Line->text().isEmpty() && mechanic2Flag == false) //mechanic_2
        {
            queryOrderDetail.prepare("UPDATE OrderDetailTable SET order_employee = ?, employee_work_hours = ?, employee_position = ? WHERE id_order = ? AND employee_position = ?");
            queryOrderDetail.addBindValue(ui->mechanic2Line->text());
            queryOrderDetail.addBindValue(ui->mechanic2HoursLine->text());
            queryOrderDetail.addBindValue("Механик_2");
            queryOrderDetail.addBindValue(orderId);
            queryOrderDetail.addBindValue("Механик_2");
            queryOrderDetail.exec();
        }
        else if (ui->mechanic2Line->text().isEmpty() && mechanic2Flag == false)
        {
            queryOrderDetail.prepare("UPDATE OrderDetailTable SET order_employee = NULL, employee_work_hours = NULL, employee_position = NULL WHERE id_order = ? AND employee_position = ?");
            queryOrderDetail.addBindValue(orderId);
            queryOrderDetail.addBindValue("Механик_2");
            queryOrderDetail.exec();
        }

        if (!ui->diagnosticianLine->text().isEmpty() && diagnosticianFlag == false) //diagnostician
        {
            queryOrderDetail.prepare("UPDATE OrderDetailTable SET order_employee = ?, employee_work_hours = ?, employee_position = ? WHERE id_order = ? AND employee_position = ?");
            queryOrderDetail.addBindValue(ui->diagnosticianLine->text());
            queryOrderDetail.addBindValue(ui->diagnosticianHoursLine->text());
            queryOrderDetail.addBindValue("Диагност");
            queryOrderDetail.addBindValue(orderId);
            queryOrderDetail.addBindValue("Диагност");
            queryOrderDetail.exec();
        }
        else if (ui->diagnosticianLine->text().isEmpty() && diagnosticianFlag == false)
        {
            queryOrderDetail.prepare("UPDATE OrderDetailTable SET order_employee = NULL, employee_work_hours = NULL, employee_position = NULL WHERE id_order = ? AND employee_position = ?");
            queryOrderDetail.addBindValue(orderId);
            queryOrderDetail.addBindValue("Диагност");
            queryOrderDetail.exec();
        }

        if (!ui->electronicsLine->text().isEmpty() && electronicFlag == false) //electronic
        {
            queryOrderDetail.prepare("UPDATE OrderDetailTable SET order_employee = ?, employee_work_hours = ?, employee_position = ? WHERE id_order = ? AND employee_position = ?");
            queryOrderDetail.addBindValue(ui->electronicsLine->text());
            queryOrderDetail.addBindValue(ui->electronicsHoursLine->text());
            queryOrderDetail.addBindValue("Электронщик");
            queryOrderDetail.addBindValue(orderId);
            queryOrderDetail.addBindValue("Электронщик");
            queryOrderDetail.exec();
        }
        else if (ui->electronicsLine->text().isEmpty() && electronicFlag == false)
        {
            queryOrderDetail.prepare("UPDATE OrderDetailTable SET order_employee = NULL, employee_work_hours = NULL, employee_position = NULL WHERE id_order = ? AND employee_position = ?");
            queryOrderDetail.addBindValue(orderId);
            queryOrderDetail.addBindValue("Электронщик");
            queryOrderDetail.exec();
        }

        if (!ui->locksmithLine->text().isEmpty() && locksmithFlag == false) //locksmith
        {
            queryOrderDetail.prepare("UPDATE OrderDetailTable SET order_employee = ?, employee_work_hours = ?, employee_position = ? WHERE id_order = ? AND employee_position = ?");
            queryOrderDetail.addBindValue(ui->locksmithLine->text());
            queryOrderDetail.addBindValue(ui->locksmithHoursLine->text());
            queryOrderDetail.addBindValue("Слесарь");
            queryOrderDetail.addBindValue(orderId);
            queryOrderDetail.addBindValue("Слесарь");
            queryOrderDetail.exec();
        }
        else if (ui->locksmithLine->text().isEmpty() && locksmithFlag == false)
        {
            queryOrderDetail.prepare("UPDATE OrderDetailTable SET order_employee = NULL, employee_work_hours = NULL, employee_position = NULL WHERE id_order = ? AND employee_position = ?");
            queryOrderDetail.addBindValue(orderId);
            queryOrderDetail.addBindValue("Слесарь");
            queryOrderDetail.exec();
        }

        if (!ui->washerLine->text().isEmpty() && washerFlag == false) //washer
        {
            queryOrderDetail.prepare("UPDATE OrderDetailTable SET order_employee = ?, employee_work_hours = ?, employee_position = ? WHERE id_order = ? AND employee_position = ?");
            queryOrderDetail.addBindValue(ui->washerLine->text());
            queryOrderDetail.addBindValue(ui->washerHoursLine->text());
            queryOrderDetail.addBindValue("Мойщик");
            queryOrderDetail.addBindValue(orderId);
            queryOrderDetail.addBindValue("Мойщик");
            queryOrderDetail.exec();
        }
        else if (ui->washerLine->text().isEmpty() && washerFlag == false)
        {
            queryOrderDetail.prepare("UPDATE OrderDetailTable SET order_employee = NULL, employee_work_hours = NULL, employee_position = NULL WHERE id_order = ? AND employee_position = ?");
            queryOrderDetail.addBindValue(orderId);
            queryOrderDetail.addBindValue("Мойщик");
            queryOrderDetail.exec();
        }
    }

    else if (queryCheckEmployee.value(0) == 0) // if not, add him
    {
        if (!ui->mechanicLine->text().isEmpty() && mechanicFlag == true)
        {
            queryOrderDetail.prepare("INSERT INTO OrderDetailTable (id_order, id_employee, order_employee, employee_work_hours, employee_position) VALUES(?, ?, ?, ?, ?)");
            queryOrderDetail.addBindValue(orderId);
            queryOrderDetail.addBindValue(employeeId);
            queryOrderDetail.addBindValue(ui->mechanicLine->text());
            queryOrderDetail.addBindValue(ui->mechanicHoursLine->text());
            queryOrderDetail.addBindValue("Механик");
            queryOrderDetail.exec();
        }

        if (!ui->mechanic2Line->text().isEmpty() && mechanic2Flag == true)
        {
            queryOrderDetail.prepare("INSERT INTO OrderDetailTable (id_order, order_employee, employee_work_hours, employee_position) VALUES(?, ?, ?, ?)");
            queryOrderDetail.addBindValue(orderId);
            queryOrderDetail.addBindValue(ui->mechanic2Line->text());
            queryOrderDetail.addBindValue(ui->mechanic2HoursLine->text());
            queryOrderDetail.addBindValue("Механик_2");
            queryOrderDetail.exec();
        }

        if (!ui->diagnosticianLine->text().isEmpty() && diagnosticianFlag == true)
        {
            queryOrderDetail.prepare("INSERT INTO OrderDetailTable (id_order, order_employee, employee_work_hours, employee_position) VALUES(?, ?, ?, ?)");
            queryOrderDetail.addBindValue(orderId);
            queryOrderDetail.addBindValue(ui->diagnosticianLine->text());
            queryOrderDetail.addBindValue(ui->diagnosticianHoursLine->text());
            queryOrderDetail.addBindValue("Диагност");
            queryOrderDetail.exec();
        }

        if (!ui->electronicsLine->text().isEmpty() && electronicFlag == true)
        {
            queryOrderDetail.prepare("INSERT INTO OrderDetailTable (id_order, order_employee, employee_work_hours, employee_position) VALUES(?, ?, ?, ?)");
            queryOrderDetail.addBindValue(orderId);
            queryOrderDetail.addBindValue(ui->electronicsLine->text());
            queryOrderDetail.addBindValue(ui->electronicsHoursLine->text());
            queryOrderDetail.addBindValue("Электронщик");
            queryOrderDetail.exec();
        }

        if (!ui->locksmithLine->text().isEmpty() && locksmithFlag == true)
        {
            queryOrderDetail.prepare("INSERT INTO OrderDetailTable (id_order, order_employee, employee_work_hours, employee_position) VALUES(?, ?, ?, ?)");
            queryOrderDetail.addBindValue(orderId);
            queryOrderDetail.addBindValue(ui->locksmithLine->text());
            queryOrderDetail.addBindValue(ui->locksmithHoursLine->text());
            queryOrderDetail.addBindValue("Слесарь");
            queryOrderDetail.exec();
        }

        if (!ui->washerLine->text().isEmpty() && washerFlag == true)
        {
            queryOrderDetail.prepare("INSERT INTO OrderDetailTable (id_order, order_employee, employee_work_hours, employee_position) VALUES(?, ?, ?, ?)");
            queryOrderDetail.addBindValue(orderId);
            queryOrderDetail.addBindValue(ui->washerLine->text());
            queryOrderDetail.addBindValue(ui->washerHoursLine->text());
            queryOrderDetail.addBindValue("Мойщик");
            queryOrderDetail.exec();
        }
    }
}

void ViewUpdateOrder::updateEmployeesTable()
{
    queryEmployeesModel->setQuery(NULL);

    loadEmployeesTable();
}

void ViewUpdateOrder::setDateAndTime()
{
    QDate currentDate = QDate::currentDate();
    QTime currentTime = QTime::currentTime();
    ui->updateDateLine->setText(currentDate.toString(Qt::SystemLocaleDate));
    ui->updateTimeLine->setText(currentTime.toString(Qt::SystemLocaleDate));
}

void ViewUpdateOrder::setValues(const QString &id)
{
    orderId = id;

    loadUsedSparePartsTable();

    QSqlQuery query(ordersDB);

    query.prepare("SELECT DISTINCT client_type, client, creation_date, creation_time, reception_date, contacts, email, auto_brand, auto_model, mileage, manufacture_year, "
                  "VIN_number, auto_license_plate, service_address, discounts, order_status, works_list, feedback "
                  "FROM OrdersHistory WHERE id_order = " + orderId);
    query.exec();
    query.next();

    QString clientType = query.value(0).toString();
    if (clientType == "Физ. лицо" || clientType == "Фіз. особа" || clientType == "Private")
        ui->clientTypeComboBox->setCurrentIndex(0);
    else if (clientType == "Юр. лицо" || clientType == "Юр. особа" || clientType == "Organization")
        ui->clientTypeComboBox->setCurrentIndex(1);

    ui->clientLine->setText(query.value(1).toString());
    ui->dateLine->setText(query.value(2).toString());
    ui->timeLine->setText(query.value(3).toString());
    ui->receptionLine->setText(query.value(4).toString());
    ui->contactLine->setText(query.value(5).toString());
    ui->emailLine->setText(query.value(6).toString());
    ui->brandLine->setText(query.value(7).toString());
    ui->modelLine->setText(query.value(8).toString());
    ui->mileageLine->setText(query.value(9).toString());
    ui->yearLine->setText(query.value(10).toString());
    ui->VIN_Line->setText(query.value(11).toString());
    ui->autoLicensePlateLine->setText(query.value(12).toString());

    QString serviceAddress = query.value(13).toString();
    if (serviceAddress == "Среднефонтанская, 30А (Приморский р-н)")
        ui->serviceComboBox->setCurrentIndex(0);
    else if (serviceAddress == "Платонова, 56 (Малиновский р-н)")
        ui->serviceComboBox->setCurrentIndex(1);
    else if (serviceAddress == "Архитекторская, 28 (Киевский р-н)")
        ui->serviceComboBox->setCurrentIndex(2);

    QString discountType = query.value(14).toString();
    if ((discountType == "Нет") || (discountType == "Немає") || (discountType == "No"))
        ui->discountsComboBox->setCurrentIndex(0);
    else if ((discountType == "Купон") || (discountType == "Coupon"))
        ui->discountsComboBox->setCurrentIndex(1);
    else if ((discountType == "Акция") || (discountType == "Акція") || (discountType == "Promotion"))
        ui->discountsComboBox->setCurrentIndex(2);
    else if ((discountType == "Особые условия") || (discountType == "Особливі умови") || (discountType == "Special conditions"))
        ui->discountsComboBox->setCurrentIndex(3);
    else if ((discountType == "Постоянный клиент") || (discountType == "Постійний клієнт") || (discountType == "Regular customer"))
        ui->discountsComboBox->setCurrentIndex(4);

    QString orderStatus = query.value(15).toString();
    if ((orderStatus == "Заявка") || (orderStatus == "Application"))
        ui->orderStatusComboBox->setCurrentIndex(0);
    else if ((orderStatus == "В работе") || (orderStatus == "В роботі") || (orderStatus == "In work"))
        ui->orderStatusComboBox->setCurrentIndex(1);
    else if ((orderStatus == "Завершен, неоплачен") || (orderStatus == "Завершений, не сплачений") || (orderStatus == "Completed, not paid"))
        ui->orderStatusComboBox->setCurrentIndex(2);
    else if ((orderStatus == "Завершен, оплачен") || (orderStatus == "Завершений, сплачений") || (orderStatus == "Completed, paid"))
        ui->orderStatusComboBox->setCurrentIndex(3);

    ui->worksList->setText(query.value(16).toString());
    ui->feedback->setText(query.value(17).toString());

    QSqlQuery queryEmployee(orderDetailDB);

    queryEmployee.prepare("SELECT order_employee, employee_work_hours, employee_position FROM OrderDetailTable WHERE id_order = " + orderId);
    queryEmployee.exec();

    while (queryEmployee.next())
    {
        QString employeePosition = queryEmployee.value(2).toString();

        if ((employeePosition == "Механик") && (ui->mechanicLine->text().isEmpty()))
        {
            ui->mechanicLine->setText(queryEmployee.value(0).toString());
            ui->mechanicHoursLine->setText(queryEmployee.value(1).toString());
        }

        else if ((employeePosition == "Механик_2") && ((!ui->mechanicLine->text().isEmpty()) || (ui->mechanic2Line->text().isEmpty())))
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

    if (ui->mechanicLine->text().isEmpty())
        mechanicFlag = true;
    if (ui->mechanic2Line->text().isEmpty())
        mechanic2Flag = true;
    if (ui->diagnosticianLine->text().isEmpty())
        diagnosticianFlag = true;
    if (ui->electronicsLine->text().isEmpty())
        electronicFlag = true;
    if (ui->locksmithLine->text().isEmpty())
        locksmithFlag = true;
    if (ui->washerLine->text().isEmpty())
        washerFlag = true;
}

void ViewUpdateOrder::on_saveUpdatedInfo_clicked()
{
    QSqlQuery queryOrders(ordersDB);

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

    queryOrders.prepare("UPDATE OrdersHistory SET client_type = ?, client = ?, updating_date = ?,  updating_time = ?, reception_date = ?, contacts = ?, email = ?, "
        "auto_brand = ?, auto_model = ?, mileage = ?, manufacture_year = ?, VIN_number = ?, auto_license_plate = ?, service_address = ?, discounts = ?, "
        "order_status = ?, works_list = ?, feedback = ? WHERE id_order = ?");

    queryOrders.addBindValue(ui->clientTypeComboBox->currentText());
    queryOrders.addBindValue(client);
    queryOrders.addBindValue(date);
    queryOrders.addBindValue(ui->updateTimeLine->text());
    queryOrders.addBindValue(ui->receptionLine->text());
    queryOrders.addBindValue(contacts);
    queryOrders.addBindValue(ui->emailLine->text());
    queryOrders.addBindValue(brandModel);
    queryOrders.addBindValue(ui->modelLine->text());
    queryOrders.addBindValue(ui->mileageLine->text());
    queryOrders.addBindValue(ui->yearLine->text());
    queryOrders.addBindValue(ui->VIN_Line->text());
    queryOrders.addBindValue(ui->autoLicensePlateLine->text());
    queryOrders.addBindValue(ui->serviceComboBox->currentText());
    queryOrders.addBindValue(ui->discountsComboBox->currentText());
    queryOrders.addBindValue(ui->orderStatusComboBox->currentText());
    queryOrders.addBindValue(ui->worksList->toPlainText());
    queryOrders.addBindValue(ui->feedback->toPlainText());
    queryOrders.addBindValue(orderId);
    queryOrders.exec();

    //int id = queryOrders.lastInsertId().toInt();

//    // Simultaneous insertion into detailed order table
//    QSqlQuery queryOrderDetail(orderDetailDB);

//    if (!ui->mechanicLine->text().isEmpty() && mechanicFlag == false) //mechanic
//    {
//        queryOrderDetail.prepare("UPDATE OrderDetailTable SET order_employee = ?, employee_work_hours = ?, employee_position = ? WHERE id_order = ? AND employee_position = ?");
//        queryOrderDetail.addBindValue(ui->mechanicLine->text());
//        queryOrderDetail.addBindValue(ui->mechanicHoursLine->text());
//        queryOrderDetail.addBindValue("Механик");
//        queryOrderDetail.addBindValue(orderId);
//        queryOrderDetail.addBindValue("Механик");
//        queryOrderDetail.exec();
//    }
//    else if (!ui->mechanicLine->text().isEmpty() && mechanicFlag == true)
//    {
//        queryOrderDetail.prepare("INSERT INTO OrderDetailTable (id_order, order_employee, employee_work_hours, employee_position) VALUES(?, ?, ?, ?)");
//        queryOrderDetail.addBindValue(orderId);
//        queryOrderDetail.addBindValue(ui->mechanicLine->text());
//        queryOrderDetail.addBindValue(ui->mechanicHoursLine->text());
//        queryOrderDetail.addBindValue("Механик");
//        queryOrderDetail.exec();
//    }
//    else if (ui->mechanicLine->text().isEmpty() && mechanicFlag == false)
//    {
//        queryOrderDetail.prepare("UPDATE OrderDetailTable SET order_employee = NULL, employee_work_hours = NULL, employee_position = NULL WHERE id_order = ? AND employee_position = ?");
//        queryOrderDetail.addBindValue(orderId);
//        queryOrderDetail.addBindValue("Механик");
//        queryOrderDetail.exec();
//    }

//    if (!ui->mechanic2Line->text().isEmpty() && mechanic2Flag == false) //mechanic_2
//    {
//        queryOrderDetail.prepare("UPDATE OrderDetailTable SET order_employee = ?, employee_work_hours = ?, employee_position = ? WHERE id_order = ? AND employee_position = ?");
//        queryOrderDetail.addBindValue(ui->mechanic2Line->text());
//        queryOrderDetail.addBindValue(ui->mechanic2HoursLine->text());
//        queryOrderDetail.addBindValue("Механик_2");
//        queryOrderDetail.addBindValue(orderId);
//        queryOrderDetail.addBindValue("Механик_2");
//        queryOrderDetail.exec();
//    }
//    else if (!ui->mechanic2Line->text().isEmpty() && mechanic2Flag == true)
//    {
//        queryOrderDetail.prepare("INSERT INTO OrderDetailTable (id_order, order_employee, employee_work_hours, employee_position) VALUES(?, ?, ?, ?)");
//        queryOrderDetail.addBindValue(orderId);
//        queryOrderDetail.addBindValue(ui->mechanic2Line->text());
//        queryOrderDetail.addBindValue(ui->mechanic2HoursLine->text());
//        queryOrderDetail.addBindValue("Механик_2");
//        queryOrderDetail.exec();
//    }
//    else if (ui->mechanic2Line->text().isEmpty() && mechanic2Flag == false)
//    {
//        queryOrderDetail.prepare("UPDATE OrderDetailTable SET order_employee = NULL, employee_work_hours = NULL, employee_position = NULL WHERE id_order = ? AND employee_position = ?");
//        queryOrderDetail.addBindValue(orderId);
//        queryOrderDetail.addBindValue("Механик_2");
//        queryOrderDetail.exec();
//    }

//    if (!ui->diagnosticianLine->text().isEmpty() && diagnosticianFlag == false) //diagnostician
//    {
//        queryOrderDetail.prepare("UPDATE OrderDetailTable SET order_employee = ?, employee_work_hours = ?, employee_position = ? WHERE id_order = ? AND employee_position = ?");
//        queryOrderDetail.addBindValue(ui->diagnosticianLine->text());
//        queryOrderDetail.addBindValue(ui->diagnosticianHoursLine->text());
//        queryOrderDetail.addBindValue("Диагност");
//        queryOrderDetail.addBindValue(orderId);
//        queryOrderDetail.addBindValue("Диагност");
//        queryOrderDetail.exec();
//    }
//    else if (!ui->diagnosticianLine->text().isEmpty() && diagnosticianFlag == true)
//    {
//        queryOrderDetail.prepare("INSERT INTO OrderDetailTable (id_order, order_employee, employee_work_hours, employee_position) VALUES(?, ?, ?, ?)");
//        queryOrderDetail.addBindValue(orderId);
//        queryOrderDetail.addBindValue(ui->diagnosticianLine->text());
//        queryOrderDetail.addBindValue(ui->diagnosticianHoursLine->text());
//        queryOrderDetail.addBindValue("Диагност");
//        queryOrderDetail.exec();
//    }
//    else if (ui->diagnosticianLine->text().isEmpty() && diagnosticianFlag == false)
//    {
//        queryOrderDetail.prepare("UPDATE OrderDetailTable SET order_employee = NULL, employee_work_hours = NULL, employee_position = NULL WHERE id_order = ? AND employee_position = ?");
//        queryOrderDetail.addBindValue(orderId);
//        queryOrderDetail.addBindValue("Диагност");
//        queryOrderDetail.exec();
//    }

//    if (!ui->electronicsLine->text().isEmpty() && electronicFlag == false) //electronic
//    {
//        queryOrderDetail.prepare("UPDATE OrderDetailTable SET order_employee = ?, employee_work_hours = ?, employee_position = ? WHERE id_order = ? AND employee_position = ?");
//        queryOrderDetail.addBindValue(ui->electronicsLine->text());
//        queryOrderDetail.addBindValue(ui->electronicsHoursLine->text());
//        queryOrderDetail.addBindValue("Электронщик");
//        queryOrderDetail.addBindValue(orderId);
//        queryOrderDetail.addBindValue("Электронщик");
//        queryOrderDetail.exec();
//    }
//    else if (!ui->electronicsLine->text().isEmpty() && electronicFlag == true)
//    {
//        queryOrderDetail.prepare("INSERT INTO OrderDetailTable (id_order, order_employee, employee_work_hours, employee_position) VALUES(?, ?, ?, ?)");
//        queryOrderDetail.addBindValue(orderId);
//        queryOrderDetail.addBindValue(ui->electronicsLine->text());
//        queryOrderDetail.addBindValue(ui->electronicsHoursLine->text());
//        queryOrderDetail.addBindValue("Электронщик");
//        queryOrderDetail.exec();
//    }
//    else if (ui->electronicsLine->text().isEmpty() && electronicFlag == false)
//    {
//        queryOrderDetail.prepare("UPDATE OrderDetailTable SET order_employee = NULL, employee_work_hours = NULL, employee_position = NULL WHERE id_order = ? AND employee_position = ?");
//        queryOrderDetail.addBindValue(orderId);
//        queryOrderDetail.addBindValue("Электронщик");
//        queryOrderDetail.exec();
//    }

//    if (!ui->locksmithLine->text().isEmpty() && locksmithFlag == false) //locksmith
//    {
//        queryOrderDetail.prepare("UPDATE OrderDetailTable SET order_employee = ?, employee_work_hours = ?, employee_position = ? WHERE id_order = ? AND employee_position = ?");
//        queryOrderDetail.addBindValue(ui->locksmithLine->text());
//        queryOrderDetail.addBindValue(ui->locksmithHoursLine->text());
//        queryOrderDetail.addBindValue("Слесарь");
//        queryOrderDetail.addBindValue(orderId);
//        queryOrderDetail.addBindValue("Слесарь");
//        queryOrderDetail.exec();
//    }
//    else if (!ui->locksmithLine->text().isEmpty() && locksmithFlag == true)
//    {
//        queryOrderDetail.prepare("INSERT INTO OrderDetailTable (id_order, order_employee, employee_work_hours, employee_position) VALUES(?, ?, ?, ?)");
//        queryOrderDetail.addBindValue(orderId);
//        queryOrderDetail.addBindValue(ui->locksmithLine->text());
//        queryOrderDetail.addBindValue(ui->locksmithHoursLine->text());
//        queryOrderDetail.addBindValue("Слесарь");
//        queryOrderDetail.exec();
//    }
//    else if (ui->locksmithLine->text().isEmpty() && locksmithFlag == false)
//    {
//        queryOrderDetail.prepare("UPDATE OrderDetailTable SET order_employee = NULL, employee_work_hours = NULL, employee_position = NULL WHERE id_order = ? AND employee_position = ?");
//        queryOrderDetail.addBindValue(orderId);
//        queryOrderDetail.addBindValue("Слесарь");
//        queryOrderDetail.exec();
//    }

//    if (!ui->washerLine->text().isEmpty() && washerFlag == false) //washer
//    {
//        queryOrderDetail.prepare("UPDATE OrderDetailTable SET order_employee = ?, employee_work_hours = ?, employee_position = ? WHERE id_order = ? AND employee_position = ?");
//        queryOrderDetail.addBindValue(ui->washerLine->text());
//        queryOrderDetail.addBindValue(ui->washerHoursLine->text());
//        queryOrderDetail.addBindValue("Мойщик");
//        queryOrderDetail.addBindValue(orderId);
//        queryOrderDetail.addBindValue("Мойщик");
//        queryOrderDetail.exec();
//    }
//    else if (!ui->washerLine->text().isEmpty() && washerFlag == true)
//    {
//        queryOrderDetail.prepare("INSERT INTO OrderDetailTable (id_order, order_employee, employee_work_hours, employee_position) VALUES(?, ?, ?, ?)");
//        queryOrderDetail.addBindValue(orderId);
//        queryOrderDetail.addBindValue(ui->washerLine->text());
//        queryOrderDetail.addBindValue(ui->washerHoursLine->text());
//        queryOrderDetail.addBindValue("Мойщик");
//        queryOrderDetail.exec();
//    }
//    else if (ui->washerLine->text().isEmpty() && washerFlag == false)
//    {
//        queryOrderDetail.prepare("UPDATE OrderDetailTable SET order_employee = NULL, employee_work_hours = NULL, employee_position = NULL WHERE id_order = ? AND employee_position = ?");
//        queryOrderDetail.addBindValue(orderId);
//        queryOrderDetail.addBindValue("Мойщик");
//        queryOrderDetail.exec();
//    }

    // Simultaneous insertion into client table
    QSqlQuery query(clientsDB);
    query.prepare("SELECT EXISTS (SELECT client_FML_name, contacts FROM ClientsTable WHERE client_FML_name = '" + ui->clientLine->text() + "' AND contacts LIKE '%" + ui->contactLine->text() + "%')");
    query.exec();
    query.next();

    if (query.value(0) != 0)
        ui->techLabel->setHidden(true);
    else if (query.value(0) == 0)
    {
        QSqlQuery queryClients(clientsDB);

        queryClients.prepare("UPDATE ClientsTable SET client_type = ?, client_FML_name = ?, contacts = ?, auto_brand = ?, auto_model = ?, mileage = ?, auto_license_plate = ?, "
            "manufacture_year = ?, VIN_number = ? WHERE id_order = ?");

        queryClients.addBindValue(ui->clientTypeComboBox->currentText());
        queryClients.addBindValue(client);
        queryClients.addBindValue(contacts);
        queryClients.addBindValue(brandModel);
        queryClients.addBindValue(ui->modelLine->text());
        queryClients.addBindValue(ui->mileageLine->text());
        queryClients.addBindValue(ui->autoLicensePlateLine->text());
        queryClients.addBindValue(ui->yearLine->text());
        queryClients.addBindValue(ui->VIN_Line->text());
        queryClients.addBindValue(orderId);
        queryClients.exec();
    }

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

    // calculate spare parts cost
    QSqlQuery querySparePartsCost(sparePartsDB);

    int sparePartsCost = 0;
    querySparePartsCost.prepare("SELECT SUM(order_spare_part_price) FROM OrderSpareParts WHERE id_order = " + orderId);
    querySparePartsCost.exec();
    if (querySparePartsCost.next())
            sparePartsCost = querySparePartsCost.value(0).toInt();
    querySparePartsCost.finish();

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

    qDebug()<<orderTotalDiscountCostDB;
    qDebug() << sparePartsCost;

    queryOrders.prepare("UPDATE OrdersHistory SET price = ? WHERE id_order = ?");
    queryOrders.addBindValue(orderTotalDiscountCostDB);
    queryOrders.addBindValue(orderId);
    queryOrders.exec();

    QDialog::close();

    QMessageBox::information(this, tr("Уведомление"), tr("Информация о заказе успешно обновлена!"), QMessageBox::Ok);
}

void ViewUpdateOrder::on_sparePartsSearch_returnPressed()
{
    searchFlag = true;

    updateSparePartsTable();
}

void ViewUpdateOrder::updateSparePartsTable()
{
    queryAvailableSparePartsModel->setQuery(NULL);

    loadSparePartsTable();
}

void ViewUpdateOrder::on_clearMechanicButton_clicked()
{
    QSqlQuery queryRemoveEmployeeModel(employeesDB);

    queryRemoveEmployeeModel.prepare("DELETE FROM OrderDetailTable WHERE id_order = ? AND order_employee = ?");
    queryRemoveEmployeeModel.addBindValue(orderId);
    queryRemoveEmployeeModel.addBindValue(ui->mechanicLine->text());
    queryRemoveEmployeeModel.exec();

    ui->mechanicLine->setText("");
    ui->mechanicHoursLine->setText("");
}

void ViewUpdateOrder::on_clearMechanic2Button_clicked()
{
    QSqlQuery queryRemoveEmployeeModel(employeesDB);

    queryRemoveEmployeeModel.prepare("DELETE FROM OrderDetailTable WHERE id_order = ? AND order_employee = ?");
    queryRemoveEmployeeModel.addBindValue(orderId);
    queryRemoveEmployeeModel.addBindValue(ui->mechanic2Line->text());
    queryRemoveEmployeeModel.exec();

    ui->mechanic2Line->setText("");
    ui->mechanic2HoursLine->setText("");
}

void ViewUpdateOrder::on_clearDiagnosticianButton_clicked()
{
    QSqlQuery queryRemoveEmployeeModel(employeesDB);

    queryRemoveEmployeeModel.prepare("DELETE FROM OrderDetailTable WHERE id_order = ? AND order_employee = ?");
    queryRemoveEmployeeModel.addBindValue(orderId);
    queryRemoveEmployeeModel.addBindValue(ui->diagnosticianLine->text());
    queryRemoveEmployeeModel.exec();

    ui->diagnosticianLine->setText("");
    ui->diagnosticianHoursLine->setText("");
}

void ViewUpdateOrder::on_clearElectronicButton_clicked()
{
    QSqlQuery queryRemoveEmployeeModel(employeesDB);

    queryRemoveEmployeeModel.prepare("DELETE FROM OrderDetailTable WHERE id_order = ? AND order_employee = ?");
    queryRemoveEmployeeModel.addBindValue(orderId);
    queryRemoveEmployeeModel.addBindValue(ui->electronicsLine->text());
    queryRemoveEmployeeModel.exec();

    ui->electronicsLine->setText("");
    ui->electronicsHoursLine->setText("");
}

void ViewUpdateOrder::on_clearLocksmithButton_clicked()
{
    QSqlQuery queryRemoveEmployeeModel(employeesDB);

    queryRemoveEmployeeModel.prepare("DELETE FROM OrderDetailTable WHERE id_order = ? AND order_employee = ?");
    queryRemoveEmployeeModel.addBindValue(orderId);
    queryRemoveEmployeeModel.addBindValue(ui->locksmithLine->text());
    queryRemoveEmployeeModel.exec();

    ui->locksmithLine->setText("");
    ui->locksmithHoursLine->setText("");
}

void ViewUpdateOrder::on_clearWasherButton_clicked()
{
    QSqlQuery queryRemoveEmployeeModel(employeesDB);

    queryRemoveEmployeeModel.prepare("DELETE FROM OrderDetailTable WHERE id_order = ? AND order_employee = ?");
    queryRemoveEmployeeModel.addBindValue(orderId);
    queryRemoveEmployeeModel.addBindValue(ui->washerLine->text());
    queryRemoveEmployeeModel.exec();

    ui->washerLine->setText("");
    ui->washerHoursLine->setText("");
}

void ViewUpdateOrder::on_updateOrderInfoButton_clicked()
{
    ui->updateOrderInfoButton->setEnabled(false);
    ui->clientTypeComboBox->setEnabled(true);
    ui->clientLine->setReadOnly(false);
    ui->contactLine->setReadOnly(false);
    ui->brandLine->setReadOnly(false);
    ui->modelLine->setReadOnly(false);
    ui->emailLine->setReadOnly(false);
    ui->yearLine->setReadOnly(false);
    ui->mileageLine->setReadOnly(false);
    ui->VIN_Line->setReadOnly(false);
    ui->autoLicensePlateLine->setReadOnly(false);
    ui->serviceComboBox->setEnabled(true);
    ui->discountsComboBox->setEnabled(true);
    ui->orderStatusComboBox->setEnabled(true);
    ui->sparePartsSearch->setReadOnly(false);
    ui->availableSparePartsTable->setEnabled(true);
    ui->employeesByServiceTable->setEnabled(true);
    ui->usedSparePartsTableView->setEnabled(true);
    ui->mechanicHoursLine->setReadOnly(false);
    ui->mechanic2HoursLine->setReadOnly(false);
    ui->diagnosticianHoursLine->setReadOnly(false);
    ui->electronicsHoursLine->setReadOnly(false);
    ui->locksmithHoursLine->setReadOnly(false);
    ui->washerHoursLine->setReadOnly(false);
    ui->clearMechanicButton->setEnabled(true);
    ui->clearMechanic2Button->setEnabled(true);
    ui->clearDiagnosticianButton->setEnabled(true);
    ui->clearElectronicButton->setEnabled(true);
    ui->clearLocksmithButton->setEnabled(true);
    ui->clearWasherButton->setEnabled(true);
    ui->worksList->setEnabled(true);
    ui->feedback->setReadOnly(false);
    ui->receptionLine->setReadOnly(false);
    ui->saveUpdatedInfo->setEnabled(true);
}
