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

    connect(ui->tableView, &QAbstractItemView::doubleClicked, this, &ListClients::showClientInfo);

    ui->searchComboBox->addItems(QStringList() << tr("Поиск по имени") << tr("Поиск по номеру"));

    searchFlag = false;

    loadTable();
    loadUserSettings();
}

ListClients::~ListClients()
{
    delete ui;
}

void ListClients::loadTable()
{
    queryModel = new QSqlQueryModel(this);

    QString queryString = "SELECT id_client, client_type, client_fml_name, contacts, email, auto_brand, auto_model, mileage, auto_license_plate, manufacture_year, "
                          "vin_number FROM clients_table ";

    QString searchString;

    if ((searchFlag) && (ui->searchComboBox->currentIndex() == 0))
        searchString.append("WHERE client_fml_name LIKE '%" + ui->clientSearch->text() + "%' GROUP BY id_client ORDER BY client_fml_name ASC");
    else if ((searchFlag) && (ui->searchComboBox->currentIndex() == 1))
        searchString.append("WHERE contacts LIKE '%" + ui->clientSearch->text() + "%' GROUP BY id_client ORDER BY client_fml_name ASC");

    queryString.append(searchString);

    queryModel->setQuery(queryString);

    queryModel->setHeaderData(0, Qt::Horizontal, tr("id"));
    queryModel->setHeaderData(1, Qt::Horizontal, tr("Тип клиента"));
    queryModel->setHeaderData(2, Qt::Horizontal, tr("ФИО клиента"));
    queryModel->setHeaderData(3, Qt::Horizontal, tr("Контакты"));
    queryModel->setHeaderData(4, Qt::Horizontal, tr("Электронная почта"));
    queryModel->setHeaderData(5, Qt::Horizontal, tr("Марка авто"));
    queryModel->setHeaderData(6, Qt::Horizontal, tr("Модель авто"));
    queryModel->setHeaderData(7, Qt::Horizontal, tr("Пробег"));
    queryModel->setHeaderData(8, Qt::Horizontal, tr("Госномер"));
    queryModel->setHeaderData(9, Qt::Horizontal, tr("Год производства"));
    queryModel->setHeaderData(10, Qt::Horizontal, tr("VIN-номер"));

    ui->tableView->setModel(queryModel);

    ui->tableView->setColumnHidden(0, true);  

    ui->tableView->horizontalHeader()->setDefaultSectionSize(maximumWidth());
    ui->tableView->resizeColumnsToContents();
    ui->tableView->resizeRowsToContents();
}

void ListClients::loadUserSettings()
{
    QString userLogin = global::getSettingsValue("userLogin", "settings").toString();
    int pos = 0;

    QRegularExpression chiefRegexp("^[1][0-9]{3}$");
    QRegularExpressionValidator chiefValidator(chiefRegexp, this);

    QRegularExpression managerRegexp("^[2][0-9]{3}$");
    QRegularExpressionValidator managerValidator(managerRegexp, this);

    if (chiefValidator.validate(userLogin, pos) || managerValidator.validate(userLogin, pos))
    {
        ui->addClientButton->setEnabled(true);
        ui->csvExportButton->setEnabled(true);
    }
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
    QDialog::close();

    QString clientId = queryModel->data(queryModel->index(index.row(), 0)).toString();

    viewUpdateClient = new ViewUpdateClient;
    viewUpdateClient->setValues(clientId);
    viewUpdateClient->show();
    viewUpdateClient->setAttribute(Qt::WA_DeleteOnClose);
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

void ListClients::saveAsCSV(QString fileName)
{
    QFile csvFile(fileName);

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
    QString fileName = QFileDialog::getSaveFileName(this, tr("Экспорт"), "", "CSV (*.csv);;All Files (*)");
    saveAsCSV(fileName);
}
