#include "ListOrders.h"
#include "ui_ListOrders.h"

ListOrders::ListOrders(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ListOrders)
{
    ui->setupUi(this);

    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowFlags(windowFlags() & Qt::WindowMinimizeButtonHint);

    ui->tableView->verticalHeader()->setSectionsClickable(false);
    ui->tableView->horizontalHeader()->setSectionsClickable(false);

    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);

    QDialog::showMaximized();

    connect(ui->tableView, &QAbstractItemView::doubleClicked, this, &ListOrders::showOrderInfo);

    searchFlag = false;

    loadTable();
    loadUserSettings();
}

ListOrders::~ListOrders()
{
    delete ui;
}

void ListOrders::loadTable()
{
    queryModel = new QSqlQueryModel(this);

    QString queryString;
    queryString = "SELECT id_order, order_status, creation_date, reception_date, client_type, client, contacts, auto_brand, auto_model, mileage, "
        "auto_license_plate, manufacture_year, vin_number, service_address, price FROM orders_history ";

    QString searchString;

    if (searchFlag)
        searchString.append("WHERE client LIKE '%" + ui->orderSearch->text() + "%' GROUP BY id_order ORDER BY client ASC");

    queryString.append(searchString);

    queryModel->setQuery(queryString);

    queryModel->setHeaderData(0, Qt::Horizontal, tr("id"));
    queryModel->insertColumn(1);
    queryModel->setHeaderData(1, Qt::Horizontal, tr("Готово"));
    queryModel->setHeaderData(2, Qt::Horizontal, tr("Статус"));
    queryModel->setHeaderData(3, Qt::Horizontal, tr("Создано"));
    queryModel->setHeaderData(4, Qt::Horizontal, tr("Дата приема"));
    queryModel->setHeaderData(5, Qt::Horizontal, tr("Тип клиента"));
    queryModel->setHeaderData(6, Qt::Horizontal, tr("ФИО клиента (название)"));
    queryModel->setHeaderData(7, Qt::Horizontal, tr("Контакты"));
    queryModel->setHeaderData(8, Qt::Horizontal, tr("Марка авто"));
    queryModel->setHeaderData(9, Qt::Horizontal, tr("Модель авто"));
    queryModel->setHeaderData(10, Qt::Horizontal, tr("Пробег"));
    queryModel->setHeaderData(11, Qt::Horizontal, tr("Госномер"));
    queryModel->setHeaderData(12, Qt::Horizontal, tr("Год выпуска"));
    queryModel->setHeaderData(13, Qt::Horizontal, tr("VIN"));
    queryModel->setHeaderData(14, Qt::Horizontal, tr("Сервис"));
    queryModel->setHeaderData(15, Qt::Horizontal, tr("Стоимость"));

    ui->tableView->setModel(queryModel);

    ui->tableView->setColumnHidden(0, true);

    for (int rowIndex = 0; rowIndex < ui->tableView->model()->rowCount(); ++rowIndex)
        ui->tableView->setIndexWidget(queryModel->index(rowIndex, 1), addCheckBoxCompleted(rowIndex));

    ui->tableView->horizontalHeader()->setDefaultSectionSize(maximumWidth());
    ui->tableView->resizeColumnsToContents();
    ui->tableView->resizeRowsToContents();
}

void ListOrders::loadUserSettings()
{
    QString userLogin = global::getSettingsValue("userLogin", "settings").toString();
    int pos = 0;

    QRegularExpression chiefRegexp("^[1][0-9]{3}$");
    QRegularExpressionValidator chiefValidator(chiefRegexp, this);

    QRegularExpression managerRegexp("^[2][0-9]{3}$");
    QRegularExpressionValidator managerValidator(managerRegexp, this);

    if (chiefValidator.validate(userLogin, pos) || managerValidator.validate(userLogin, pos))
    {
        ui->orderCreationButton->setEnabled(true);
        ui->csvExportButton->setEnabled(true);        
    }
}

QWidget* ListOrders::addCheckBoxCompleted(int rowIndex)
{
    QWidget *widget = new QWidget(this);
    QHBoxLayout *layout = new QHBoxLayout(widget);
    QCheckBox *checkBox = new QCheckBox(widget);

    layout->addWidget(checkBox, 0, Qt::AlignCenter);

    queryModelCheckBox = new QSqlQueryModel(this);

    QString queryStringCheckBox = "SELECT is_ready FROM orders_history";

    queryModelCheckBox->setQuery(queryStringCheckBox);

    QString isFulfilled = queryModelCheckBox->data(queryModelCheckBox->index(rowIndex, 0), Qt::EditRole).toString();

    // set checked/unchecked in tableView
    if (isFulfilled == "1")
        checkBox->setChecked(true);
    else
        checkBox->setChecked(false);

    connect(checkBox, &QAbstractButton::pressed, this, &ListOrders::checkBoxStateChanged);

    QString id = queryModel->data(queryModel->index(rowIndex, 0), Qt::EditRole).toString();

    checkBox->setProperty("checkBox", QVariant::fromValue(checkBox));
    checkBox->setProperty("id",       QVariant::fromValue(id));

    return widget;
}

void ListOrders::checkBoxStateChanged()
{
    QString id = sender()->property("id").value<QString>();
    QCheckBox *checkBox = sender()->property("checkBox").value<QCheckBox*>();

    QSqlQuery query(ordersHistoryTable);

    if (!checkBox->isChecked())
    {
        checkBox->setChecked(true);

        query.prepare("UPDATE orders_history SET is_ready = 1 WHERE id_order = ?");
        query.addBindValue(id);
        query.exec();
    }
    else if (checkBox->isChecked())
    {
        checkBox->setChecked(false);

        query.prepare("UPDATE orders_history SET is_ready = 0 WHERE id_order = ?");
        query.addBindValue(id);
        query.exec();
    }

    on_updateButton_clicked();
}

void ListOrders::on_orderCreationButton_clicked()
{
    QDialog::close();

    addOrder = new AddOrder;
    addOrder->show();
    addOrder->setAttribute(Qt::WA_DeleteOnClose);
}

void ListOrders::showOrderInfo(const QModelIndex &index)
{
    QDialog::hide();

    QString orderId = queryModel->data(queryModel->index(index.row(), 0)).toString();

    viewUpdateOrder = new ViewUpdateOrder;
    viewUpdateOrder->setValues(orderId);
    viewUpdateOrder->show();
    viewUpdateOrder->setAttribute(Qt::WA_DeleteOnClose);
}

void ListOrders::on_updateButton_clicked()
{
    queryModel->setQuery(NULL);

    loadTable();
}

void ListOrders::on_orderSearch_returnPressed()
{
    if (ui->orderSearch->text().isEmpty())
        searchFlag = false;
    else
        searchFlag = true;

    on_updateButton_clicked();
}

void ListOrders::saveAsCSV(QString fileName)
{
    QFile csvFile (fileName);

    if (csvFile.open(QIODevice::WriteOnly))
    {
        QTextStream textStream(&csvFile);
        QStringList stringList;

        stringList << "\" \"";
        for (int column = 1; column < ui->tableView->horizontalHeader()->count(); ++column)
            stringList << "\"" + ui->tableView->model()->headerData(column, Qt::Horizontal).toString() + "\"";

        textStream << stringList.join(";") + "\n";

        for (int row = 0; row < ui->tableView->verticalHeader()->count(); ++row)
        {
            stringList.clear();
            stringList << "\"" + ui->tableView->model()->headerData(row, Qt::Vertical).toString() + "\"";

            for (int column = 1; column < ui->tableView->horizontalHeader()->count(); ++column)
                stringList << "\"" + ui->tableView->model()->data(ui->tableView->model()->index(row, column), Qt::DisplayRole).toString() + "\"";

            textStream << stringList.join(";") + "\n";
        }

        csvFile.close();

        QMessageBox::information(this, tr("Уведомление"), tr("База заказов успешно экспортирована!"), QMessageBox::Ok);
    }
}

void ListOrders::on_csvExportButton_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Экспорт"), "", "CSV (*.csv);;All Files (*)");
    saveAsCSV(fileName);
}
