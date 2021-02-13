#include "UpdateClients.h"
#include "ui_UpdateClients.h"

UpdateClients::UpdateClients(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UpdateClients)
{
    ui->setupUi(this);

    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowFlags(windowFlags() & Qt::WindowMinimizeButtonHint);

    ui->autoErrorLabel->setStyleSheet("color: transparent");
    ui->contactsErrorLabel->setStyleSheet("color: transparent");
    ui->nameErrorLabel->setStyleSheet("color: transparent");

    ui->clientTypeComboBox->addItems(QStringList() << tr("Физ. лицо") << tr("Юр. лицо"));
}

UpdateClients::~UpdateClients()
{
    delete ui;
}

void UpdateClients::closeEvent(QCloseEvent*)
{
    QDialog::close();

    listClients = new ListClients;
    listClients->show();
    listClients->setAttribute(Qt::WA_DeleteOnClose);
}

void UpdateClients::on_saveUpdatedInfo_clicked()
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

    query.prepare("UPDATE ClientsTable SET client_type = ?, client_FML_name = ?, contacts = ?, email = ?, auto_brand = ?, auto_model = ?, auto_license_plate = ?, manufacture_year = ?, "
        "VIN_number = ? WHERE id_client = ?");

    query.addBindValue(ui->clientTypeComboBox->currentText());
    query.addBindValue(clientFMLname);
    query.addBindValue(clientContacts);
    query.addBindValue(ui->emailLine->text());
    query.addBindValue(autoBrand);
    query.addBindValue(ui->autoModelLine->text());
    query.addBindValue(ui->autoLicensePlateLine->text());
    query.addBindValue(ui->manufactureYearLine->text());
    query.addBindValue(ui->VINnumberLine->text());
    query.addBindValue(clientId);
    query.exec();

    QDialog::close();

    QMessageBox::information(this, tr("Уведомление"), tr("Информация о клиенте успешно обновлена!"), QMessageBox::Ok);
}

void UpdateClients::setValues(const QString &id)
{
    clientId = id;

    QSqlQuery query(clientsDB);

    query.prepare("SELECT DISTINCT client_type, client_FML_name, contacts, email, auto_brand, auto_model, auto_license_plate, manufacture_year, "
        "VIN_number FROM ClientsTable WHERE id_client = " + clientId);

    query.exec();
    query.next();

    QString clientType = query.value(0).toString();
    if (clientType == "Физ. лицо")
        ui->clientTypeComboBox->setCurrentIndex(0);
    else if (clientType == "Юр. лицо")
        ui->clientTypeComboBox->setCurrentIndex(1);

    ui->clientFMLnameLine->setText(query.value(1).toString());
    QString contacts = query.value(2).toString();
    contacts.replace("\n", ", ");
    ui->contactsLine->setText(contacts);
    ui->emailLine->setText(query.value(3).toString());
    ui->autoBrandLine->setText(query.value(4).toString());
    ui->autoModelLine->setText(query.value(5).toString());
    ui->autoLicensePlateLine->setText(query.value(6).toString());
    ui->manufactureYearLine->setText(query.value(7).toString());
    ui->VINnumberLine->setText(query.value(8).toString());
}

// back to ViewClient
void UpdateClients::on_backToViewInfoButton_clicked()
{
    emit sendData(false);

    QDialog::close();
}
