#include "ViewClient.h"
#include "ui_ViewClient.h"

ViewClient::ViewClient(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ViewClient)
{
    ui->setupUi(this);

    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowFlags(windowFlags() & Qt::WindowMinimizeButtonHint);
}

ViewClient::~ViewClient()
{
    delete ui;
}

void ViewClient::closeEvent(QCloseEvent*)
{
    QDialog::hide();

    listClient = new ListClients;
    listClient->show();
    listClient->setAttribute(Qt::WA_DeleteOnClose);
}

// Gets client's id from ListClients
void ViewClient::setValues(const QString &id)
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

void ViewClient::on_updateClientInfoButton_clicked()
{
    QDialog::hide();

    updateClients = new UpdateClients;
    updateClients->setValues(clientId);
    connect(updateClients, &UpdateClients::sendData, this, &ViewClient::receiveData);
    updateClients->show();
    updateClients->setAttribute(Qt::WA_DeleteOnClose);
}

void ViewClient::receiveData(bool update)
{
    if (update)
        QDialog::close();
    else
        QDialog::show();
}
