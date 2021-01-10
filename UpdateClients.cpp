#include "UpdateClients.h"
#include "ui_UpdateClients.h"

UpdateClients::UpdateClients(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UpdateClients)
{
    ui->setupUi(this);

    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowFlags(windowFlags() & Qt::WindowMinimizeButtonHint);
}

UpdateClients::~UpdateClients()
{
    delete ui;
}

void UpdateClients::on_saveUpdatedInfo_clicked()
{
    QSqlQuery query(clientsDB);

    QString clientFMLname = ui->clientFMLnameLine->text();
    QString clientContacts = ui->contactsLine->text();
    QString autoModel = ui->autoModelLine->text();
    QString autoLicensePlate = ui->autoLicensePlateLine->text();
    QString manufactureYear = ui->manufactureYearLine->text();
    QString VIN_Number = ui->VINnumberLine->text();

    query.prepare("UPDATE ClientsTable SET client_FML_name = ?, contacts = ?, auto_model = ?, auto_license_plate = ?, manufacture_year = ?, "
        "VIN_number = ? WHERE id_client = ?");

    query.addBindValue(clientFMLname);
    query.addBindValue(clientContacts);
    query.addBindValue(autoModel);
    query.addBindValue(autoLicensePlate);
    query.addBindValue(manufactureYear);
    query.addBindValue(VIN_Number);
    query.addBindValue(clientId);
    query.exec();

    QDialog::close();

    QMessageBox::information(this, tr("Уведомление"), tr("Информация о клиенте успешно обновлена!"), QMessageBox::Ok);
}

void UpdateClients::setValues(const QString& id)
{
    clientId = id;

    QSqlQuery query(clientsDB);

    query.prepare("SELECT DISTINCT client_FML_name, contacts, auto_model, auto_license_plate, manufacture_year, VIN_number "
        "FROM ClientsTable WHERE id_client = " + clientId);

    query.exec();
    query.next();

    ui->clientFMLnameLine->setText(query.value(0).toString());
    ui->contactsLine->setText(query.value(1).toString());
    ui->autoModelLine->setText(query.value(2).toString());
    ui->autoLicensePlateLine->setText(query.value(3).toString());
    ui->manufactureYearLine->setText(query.value(4).toString());
    ui->VINnumberLine->setText(query.value(5).toString());
}

// back to ViewClient
void UpdateClients::on_backToViewInfoButton_clicked()
{
    emit sendData(false);

    QDialog::close();
}
