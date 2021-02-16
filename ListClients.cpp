#include "ListClients.h"
#include "ui_ListClients.h"

ListClients::ListClients(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ListClients)
{
    ui->setupUi(this);

    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowFlags(windowFlags() & Qt::WindowMinimizeButtonHint);

    QDialog::showMaximized();

    ui->tableView->verticalHeader()->setSectionsClickable(false);
    ui->tableView->horizontalHeader()->setSectionsClickable(false);

    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);

    QDir tempDirDB = QDir::currentPath(); tempDirDB.cdUp(); QString dirDB = tempDirDB.path();

    clientsDB = QSqlDatabase::addDatabase("QSQLITE");
    clientsDB.setDatabaseName(dirDB + "\\CRM_AutoService\\ServiceStationDB.db");
    clientsDB.open();

    connect(ui->tableView, &QAbstractItemView::doubleClicked, this, &ListClients::showClientInfo);

    ui->searchComboBox->addItems(QStringList() << tr("Поиск по имени") << tr("Поиск по номеру"));

    searchFlag = false;

    loadTable();
}

ListClients::~ListClients()
{
    delete ui;
}

void ListClients::loadTable()
{
    queryModel = new QSqlQueryModel(this);

    QString queryString = "SELECT id_client, client_type, client_FML_name, contacts, email, auto_model, auto_license_plate, manufacture_year, "
                          "VIN_number FROM ClientsTable ";

    QString searchString;

    if (searchFlag && ui->searchComboBox->currentIndex() == 0)
        searchString.append("WHERE client_FML_name LIKE '%" + ui->clientSearch->text() + "%' GROUP BY id_client ORDER BY client_FML_name ASC");
    else if (searchFlag && ui->searchComboBox->currentIndex() == 1)
        searchString.append("WHERE contacts LIKE '%" + ui->clientSearch->text() + "%' GROUP BY id_client ORDER BY client_FML_name ASC");

    queryString.append(searchString);

    queryModel->setQuery(queryString);

    queryModel->setHeaderData(0, Qt::Horizontal, tr("id"));
    queryModel->setHeaderData(1, Qt::Horizontal, tr("Тип клиента"));
    queryModel->setHeaderData(2, Qt::Horizontal, tr("ФИО клиента"));
    queryModel->setHeaderData(3, Qt::Horizontal, tr("Контакты"));
    queryModel->setHeaderData(4, Qt::Horizontal, tr("Электронная почта"));
    queryModel->setHeaderData(5, Qt::Horizontal, tr("Модель авто"));
    queryModel->setHeaderData(6, Qt::Horizontal, tr("Госномер"));
    queryModel->setHeaderData(7, Qt::Horizontal, tr("Год производства"));
    queryModel->setHeaderData(8, Qt::Horizontal, tr("VIN-номер"));

    ui->tableView->setModel(queryModel);

    ui->tableView->setColumnHidden(0, true);

    ui->tableView->horizontalHeader()->setDefaultSectionSize(maximumWidth());
    ui->tableView->resizeColumnsToContents();
    ui->tableView->resizeRowsToContents();
}

void ListClients::on_addClientButton_clicked()
{
    QDialog::close();

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

void ListClients::on_clientSearch_returnPressed()
{
    if (ui->clientSearch->text().isEmpty())
        searchFlag = false;
    else
        searchFlag = true;

    on_updateButton_clicked();
}

void ListClients::on_updateButton_clicked()
{
    queryModel->setQuery(NULL);

    loadTable();
}

void ListClients::saveAsCSV(QString filename)
{
    QFile csvFile (filename);

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

        QMessageBox::information(this, tr("Уведомление"), tr("База клиентов успешно экспортирована!"), QMessageBox::Ok);
    }
}

void ListClients::on_csvExportButton_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Экспорт"), "", tr("CSV (*.csv);;All Files (*)"));
    saveAsCSV(fileName);
}
