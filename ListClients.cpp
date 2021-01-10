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

    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);

    clientsDB = QSqlDatabase::addDatabase("QSQLITE");
    clientsDB.setDatabaseName("C:\\Users\\BohdanF\\Documents\\Diploma\\CRM_AutoService\\ServiceStationDB.db");
    clientsDB.open();

    connect(ui->tableView, &QAbstractItemView::doubleClicked, this, &ListClients::showClientInfo);

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

    queryString = "SELECT id_client, client_FML_name, contacts, auto_model, auto_license_plate, manufacture_year, "
        "VIN_number FROM ClientsTable";

    queryModel->setQuery(queryString, clientsTable);

    queryModel->setHeaderData(0, Qt::Horizontal, tr("id"));
    queryModel->setHeaderData(1, Qt::Horizontal, tr("ФИО клиента"));
    queryModel->setHeaderData(2, Qt::Horizontal, tr("Контакты"));
    queryModel->setHeaderData(3, Qt::Horizontal, tr("Модель авто"));
    queryModel->setHeaderData(4, Qt::Horizontal, tr("Госномер"));
    queryModel->setHeaderData(5, Qt::Horizontal, tr("Год производства"));
    queryModel->setHeaderData(6, Qt::Horizontal, tr("VIN-номер"));

    ui->tableView->setModel(queryModel);

    ui->tableView->setColumnHidden(0, true);

    ui->tableView->horizontalHeader()->setDefaultSectionSize(maximumWidth());
    ui->tableView->resizeColumnsToContents();
}

void ListClients::on_addClientButton_clicked()
{
    addClient = new AddClient;
    addClient->show();
    addClient->setAttribute(Qt::WA_DeleteOnClose);
}

void ListClients::showClientInfo(const QModelIndex &index)
{
    QDialog::hide();

    QString clientId = queryModel->data(queryModel->index(index.row(), 0)).toString();

    viewClient = new ViewClient;
    viewClient->setValues(clientId);
    viewClient->show();
    viewClient->setAttribute(Qt::WA_DeleteOnClose);
}
