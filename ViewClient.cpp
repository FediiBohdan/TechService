#include "ViewClient.h"
#include "ui_ViewClient.h"

ViewClient::ViewClient(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ViewClient)
{
    ui->setupUi(this);

    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowFlags(windowFlags() & Qt::WindowMinimizeButtonHint);

    ui->tableView->verticalHeader()->setSectionsClickable(false);
    ui->tableView->horizontalHeader()->setSectionsClickable(false);

    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);

    QDir tempDirDB = QDir::currentPath(); tempDirDB.cdUp(); QString dirDB = tempDirDB.path();

    clientsDB = QSqlDatabase::addDatabase("QSQLITE");
    clientsDB.setDatabaseName(dirDB + "\\CRM_AutoService\\ServiceStationDB.db");
    clientsDB.open();
}

ViewClient::~ViewClient()
{
    delete ui;
}

void ViewClient::closeEvent(QCloseEvent*)
{
    QDialog::close();

    listClient = new ListClients;
    listClient->show();
    listClient->setAttribute(Qt::WA_DeleteOnClose);
}

// Gets client's id from ListClients
void ViewClient::setValues(const QString &id)
{
    clientId = id;

    QSqlQuery query(clientsDB);

    query.prepare("SELECT DISTINCT client_type, client_FML_name, contacts, email, auto_brand, auto_model, auto_license_plate, manufacture_year, VIN_number "
        "FROM ClientsTable WHERE id_client = " + clientId);

    query.exec();
    query.next();

    ui->clientTypeLine->setText(query.value(0).toString());
    ui->clientFMLname->setText(query.value(1).toString());
    QString contacts = query.value(2).toString();
    contacts.replace("\n", ", ");
    ui->contacts->setText(contacts);
    ui->email->setText(query.value(3).toString());
    ui->autoBrand->setText(query.value(4).toString());
    ui->autoModel->setText(query.value(5).toString());
    ui->autoLicensePlate->setText(query.value(6).toString());
    ui->manufactureYear->setText(query.value(7).toString());
    ui->VINnumber->setText(query.value(8).toString());

    loadOrderHistoryTable();
}

void ViewClient::on_updateClientInfoButton_clicked()
{
    QDialog::hide();

    updateClients = new UpdateClients;
    updateClients->setValues(clientId);
    connect(updateClients, &UpdateClients::sendData, this, &ViewClient::receiveData);
    updateClients->show();
    updateClients->setAttribute(Qt::WA_DeleteOnClose);
}

void ViewClient::loadOrderHistoryTable()
{
    queryModel = new QSqlQueryModel(this);

    QString queryString;
    queryString = "SELECT id_order, order_status, creation_date, client_type, client, auto_brand, auto_model, "
        "service_address, price FROM OrdersHistory WHERE client = '" + ui->clientFMLname->text() + "' AND contacts LIKE '%" + ui->contacts->text() +"%'";

    queryModel->setQuery(queryString);

    queryModel->setHeaderData(0, Qt::Horizontal, tr("id"));
    queryModel->setHeaderData(1, Qt::Horizontal, tr("Статус"));
    queryModel->setHeaderData(2, Qt::Horizontal, tr("Создано"));
    queryModel->setHeaderData(3, Qt::Horizontal, tr("Тип клиента"));
    queryModel->setHeaderData(4, Qt::Horizontal, tr("ФИО клиента (название)"));
    queryModel->setHeaderData(5, Qt::Horizontal, tr("Марка авто"));
    queryModel->setHeaderData(6, Qt::Horizontal, tr("Модель авто"));
    queryModel->setHeaderData(7, Qt::Horizontal, tr("Сервис"));
    queryModel->setHeaderData(8, Qt::Horizontal, tr("Стоимость"));

    ui->tableView->setModel(queryModel);

    ui->tableView->setColumnHidden(0, true);

    ui->tableView->horizontalHeader()->setDefaultSectionSize(maximumWidth());
    ui->tableView->resizeColumnsToContents();
    ui->tableView->resizeRowsToContents();
}

void ViewClient::receiveData(bool update)
{
    if (update)
        QDialog::close();
    else
        QDialog::show();
}

void ViewClient::on_createOrderByClientButton_clicked()
{
    QDialog::close();

    QString clientName = ui->clientFMLname->text();
    QString contacts = ui->contacts->text();
    QString autoBrand = ui->autoBrand->text();

    addOrder = new AddOrder;
    addOrder->setValues(clientName, contacts, autoBrand);
    addOrder->show();
    addOrder->setAttribute(Qt::WA_DeleteOnClose);
}
