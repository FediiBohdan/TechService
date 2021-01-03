#include "ListClients.h"
#include "ui_ListClients.h"

ListClients::ListClients(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ListClients)
{
    ui->setupUi(this);

    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowFlags(windowFlags() & Qt::WindowMinimizeButtonHint);

    ui->tableView->verticalHeader()->setSectionsClickable(false);
    ui->tableView->horizontalHeader()->setSectionsClickable(false);

    clientsDB = QSqlDatabase::addDatabase("QSQLITE");
    clientsDB.setDatabaseName("D:\\Diploma\\CRM_AutoService\\ServiceStationDB.db");
    clientsDB.open();

    loadTable();
}

ListClients::~ListClients()
{
    delete ui;
}

void ListClients::loadTable()
{
    queryModel = new QSqlQueryModel(this);

    QString queryString;

    queryString = "SELECT id_client, client_FML_name, contacts, auto_model, auto_license_plate, manufacture_year, VIN_number FROM ClientsTable";

    queryModel->setQuery(queryString, clientsTable);

    queryModel->setHeaderData(0, Qt::Horizontal, tr("id"));
    queryModel->setHeaderData(1, Qt::Horizontal, tr("ФИО клиента"));
    queryModel->setHeaderData(2, Qt::Horizontal, tr("Контакты"));
    queryModel->setHeaderData(3, Qt::Horizontal, tr("Модель авто"));
    queryModel->setHeaderData(4, Qt::Horizontal, tr("Госномер"));
    queryModel->setHeaderData(5, Qt::Horizontal, tr("Год производства"));
    queryModel->setHeaderData(6, Qt::Horizontal, tr("VIN-номер"));

    ui->tableView->setModel(queryModel);

    for (qint32 row_index = 0; row_index < ui->tableView->model()->rowCount(); ++row_index)
    {
        QString id_client = queryModel->data(queryModel->index(row_index, 0)).toString();
        QString clientPersonalInfo = queryModel->data(queryModel->index(row_index, 1)).toString();
        QString clientContacts = queryModel->data(queryModel->index(row_index, 2)).toString();
        QString autoModel = queryModel->data(queryModel->index(row_index, 3)).toString();
        QString autoLicensePlate = queryModel->data(queryModel->index(row_index, 4)).toString();
        QString manufactureYear = queryModel->data(queryModel->index(row_index, 5)).toString();
        QString VIN_number = queryModel->data(queryModel->index(row_index, 6)).toString();

        QSqlQuery query(clientsDB);
    }

    ui->tableView->horizontalHeader()->setDefaultSectionSize(maximumWidth());
    ui->tableView->resizeColumnsToContents();
}

void ListClients::on_addClientButton_clicked()
{
    addClient = new AddClient;
    addClient->show();
    addClient->setAttribute(Qt::WA_DeleteOnClose);
}
