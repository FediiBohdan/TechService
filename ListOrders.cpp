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

    QDir tempDirDB = QDir::currentPath(); tempDirDB.cdUp(); QString dirDB = tempDirDB.path();

    ordersHistoryDB = QSqlDatabase::addDatabase("QSQLITE");
    ordersHistoryDB.setDatabaseName(dirDB + "\\CRM_AutoService\\ServiceStationDB.db");
    ordersHistoryDB.open();

    connect(ui->tableView, &QAbstractItemView::doubleClicked, this, &ListOrders::showOrderInfo);

    searchFlag = false;

    loadTable();
}

ListOrders::~ListOrders()
{
    delete ui;
}

void ListOrders::loadTable()
{
    queryModel = new QSqlQueryModel(this);

    QString queryString;
    queryString = "SELECT id_order, creation_date, updating_date, client, contacts, auto_model, auto_license_plate, "
        "manufacture_year, VIN_number, service_address, work_hours, price FROM OrdersHistory ";

    QString searchString;

    if (searchFlag)
        searchString.append("WHERE client LIKE '%" + ui->orderSearch->text() + "%' GROUP BY id_order ORDER BY client ASC");

    queryString.append(searchString);

    queryModel->setQuery(queryString);

    queryModel->setHeaderData(0, Qt::Horizontal, tr("id"));
    queryModel->insertColumn(1);
    queryModel->setHeaderData(1, Qt::Horizontal, tr("Готово"));
    queryModel->setHeaderData(2, Qt::Horizontal, tr("Дата создания"));
    queryModel->setHeaderData(3, Qt::Horizontal, tr("Дата обновления"));
    queryModel->setHeaderData(4, Qt::Horizontal, tr("ФИО клиента"));
    queryModel->setHeaderData(5, Qt::Horizontal, tr("Контакты"));
    queryModel->setHeaderData(6, Qt::Horizontal, tr("Модель авто"));
    queryModel->setHeaderData(7, Qt::Horizontal, tr("Госномер"));
    queryModel->setHeaderData(8, Qt::Horizontal, tr("Год выпуска"));
    queryModel->setHeaderData(9, Qt::Horizontal, tr("VIN"));
    queryModel->setHeaderData(10, Qt::Horizontal, tr("Сервис"));
    queryModel->setHeaderData(11, Qt::Horizontal, tr("Часы работы"));
    queryModel->setHeaderData(12, Qt::Horizontal, tr("Стоимость"));

    ui->tableView->setModel(queryModel);

    ui->tableView->setColumnHidden(0, true);

    for (int rowIndex = 0; rowIndex < ui->tableView->model()->rowCount(); ++rowIndex)
    {
        //ui->tableView->setIndexWidget(queryModel->index(rowIndex, 3), addWidgetContent(rowIndex));
        ui->tableView->setIndexWidget(queryModel->index(rowIndex, 1), addCheckBoxCompleted(rowIndex));
    }

    ui->tableView->horizontalHeader()->setDefaultSectionSize(maximumWidth());
    ui->tableView->resizeColumnsToContents();
    ui->tableView->resizeRowsToContents();
}

QWidget* ListOrders::addCheckBoxCompleted(int rowIndex)
{
    QWidget *widget = new QWidget(this);
    QHBoxLayout *layout = new QHBoxLayout(widget);
    QCheckBox *checkBox = new QCheckBox(widget);

    layout->addWidget(checkBox, 0, Qt::AlignCenter);

    queryModelCheckBox = new QSqlQueryModel(this);

    QString queryStringCheckBox = "SELECT is_ready FROM OrdersHistory";

    queryModelCheckBox->setQuery(queryStringCheckBox, ordersHistoryTable);

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

        query.prepare("UPDATE OrdersHistory SET is_ready = 1 WHERE id_order = ?");
        query.addBindValue(id);
        query.exec();
    }
    else if (checkBox->isChecked())
    {
        checkBox->setChecked(false);

        query.prepare("UPDATE OrdersHistory SET is_ready = 0 WHERE id_order = ?");
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

    viewOrders = new ViewOrders;
    viewOrders->setValues(orderId);
    viewOrders->show();
    viewOrders->setAttribute(Qt::WA_DeleteOnClose);
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
