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
}

AddClient::~AddClient()
{
    delete ui;
}

void AddClient::on_saveClientButton_clicked()
{
    QSqlQuery query(clientsDB);

    QString clientFMLname = ui->clientFMLnameLine->text();
    QString clientContacts = ui->contactsLine->text();
    QString autoModel = ui->autoModelLine->text();
    QString autoLicensePlate = ui->autoLicensePlateLine->text();
    QString manufactureYear = ui->manufactureYearLine->text();
    QString VIN_Number = ui->VINnumberLine->text();

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

    if (autoModel.isEmpty())
    {
        error = true; ui->autoErrorLabel->setStyleSheet("color: red");
    }
    else
        ui->autoErrorLabel->setStyleSheet("color: transparent");

    if (error)
        return;

    query.prepare("INSERT INTO ClientsTable (client_FML_name, contacts, auto_model, auto_license_plate, manufacture_year, VIN_Number)"
        "VALUES(?, ?, ?, ?, ?, ?)");

    query.addBindValue(clientFMLname);
    query.addBindValue(clientContacts);
    query.addBindValue(autoModel);
    query.addBindValue(autoLicensePlate);
    query.addBindValue(manufactureYear);
    query.addBindValue(VIN_Number);
    query.exec();

    QDialog::close();

    QMessageBox::information(this, tr("Уведомление"), tr("Клиент успешно добавлен!"), QMessageBox::Ok);
}
