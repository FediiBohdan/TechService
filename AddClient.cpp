#include "AddClient.h"
#include "ui_AddClient.h"

AddClient::AddClient(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddClient)
{
    ui->setupUi(this);

    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowFlags(windowFlags() & Qt::WindowMinimizeButtonHint);

    QDir tempDirDB = QDir::currentPath(); tempDirDB.cdUp(); QString dirDB = tempDirDB.path();

    clientsDB = QSqlDatabase::addDatabase("QSQLITE");
    clientsDB.setDatabaseName(dirDB + "\\CRM_AutoService\\ServiceStationDB.db");
    clientsDB.open();

    ui->autoErrorLabel->setStyleSheet("color: transparent");
    ui->contactsErrorLabel->setStyleSheet("color: transparent");
    ui->nameErrorLabel->setStyleSheet("color: transparent");

    ui->clientTypeComboBox->addItems(QStringList() << tr("Физ. лицо") << tr("Юр. лицо"));
}

AddClient::~AddClient()
{
    delete ui;
}

void AddClient::closeEvent(QCloseEvent *)
{
    QDialog::close();

    listClients = new ListClients;
    listClients->show();
    listClients->setAttribute(Qt::WA_DeleteOnClose);
}

void AddClient::on_saveClientButton_clicked()
{
    QSqlQuery query(clientsDB);

    QString clientFMLname = ui->clientFMLnameLine->text();
    QString clientContacts = ui->contactsLine->text();
    QString autoBrand = ui->autoBrandLine->text();

    bool error = false;

    if (clientFMLname.isEmpty())
    {
        error = true; ui->nameErrorLabel->setStyleSheet("color: red");
    }
    else
        ui->nameErrorLabel->setStyleSheet("color: transparent");

    if (clientContacts.isEmpty())
    {
        error = true; ui->contactsErrorLabel->setStyleSheet("color: red");
    }
    else
        ui->contactsErrorLabel->setStyleSheet("color: transparent");

    if (autoBrand.isEmpty())
    {
        error = true; ui->autoErrorLabel->setStyleSheet("color: red");
    }
    else
        ui->autoErrorLabel->setStyleSheet("color: transparent");

    if (error)
        return;

    clientContacts.replace(", ", "\n");

    query.prepare("INSERT INTO ClientsTable (client_type, client_FML_name, contacts, email, auto_brand, auto_model, mileage, auto_license_plate, manufacture_year, VIN_Number)"
        "VALUES(?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");

    query.addBindValue(ui->clientTypeComboBox->currentText());
    query.addBindValue(clientFMLname);
    query.addBindValue(clientContacts);
    query.addBindValue(ui->emailLine->text());
    query.addBindValue(autoBrand);
    query.addBindValue(ui->autoModelLine->text());
    query.addBindValue(ui->mileageLine->text());
    query.addBindValue(ui->autoLicensePlateLine->text());
    query.addBindValue(ui->manufactureYearLine->text());
    query.addBindValue(ui->VINnumberLine->text());
    query.exec();

    QDialog::close();

    QMessageBox::information(this, tr("Уведомление"), tr("Клиент успешно добавлен!"), QMessageBox::Ok);
}
