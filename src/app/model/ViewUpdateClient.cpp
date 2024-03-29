#include "ViewUpdateClient.h"
#include "ui_ViewUpdateClient.h"

ViewUpdateClient::ViewUpdateClient(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ViewUpdateClient)
{
    ui->setupUi(this);

    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowFlags(windowFlags() & Qt::WindowMinimizeButtonHint);

    ui->autoErrorLabel->setStyleSheet("color: transparent");
    ui->contactsErrorLabel->setStyleSheet("color: transparent");
    ui->nameErrorLabel->setStyleSheet("color: transparent");

    ui->clientTypeComboBox->addItems(QStringList() << tr("Физ. лицо") << tr("Юр. лицо"));
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);

    loadUserSettings();
}

ViewUpdateClient::~ViewUpdateClient()
{
    delete ui;
}

/**
 * Event on window close.
 */
void ViewUpdateClient::closeEvent(QCloseEvent *)
{
    QDialog::close();

    listClients = new ListClients;
    listClients->show();
    listClients->setAttribute(Qt::WA_DeleteOnClose);
}

/**
 * Checks input information and updates client in DB.
 */
void ViewUpdateClient::on_saveUpdatedInfo_clicked()
{
    QSqlQuery query(clientsTable);

    QString clientFMLname = ui->clientFMLname->text();
    QString clientContacts = ui->contacts->text();
    QString autoBrand = ui->autoBrand->text();

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

    query.prepare("UPDATE clients_table SET client_type = ?, client_fml_name = ?, contacts = ?, email = ?, auto_brand = ?, auto_model = ?, mileage = ?, auto_license_plate = ?, manufacture_year = ?, "
                  "vin_number = ? WHERE id_client = ?");
    query.addBindValue(ui->clientTypeComboBox->currentText());
    query.addBindValue(clientFMLname);
    query.addBindValue(clientContacts);
    query.addBindValue(ui->email->text());
    query.addBindValue(autoBrand);
    query.addBindValue(ui->autoModel->text());
    query.addBindValue(ui->mileageLine->text());
    query.addBindValue(ui->autoLicensePlate->text());
    query.addBindValue(ui->manufactureYear->text());
    query.addBindValue(ui->VINnumber->text());
    query.addBindValue(clientId);
    query.exec();

    QDialog::close();

    QMessageBox::information(this, tr("Уведомление"), tr("Информация о клиенте успешно обновлена!"), QMessageBox::Ok);
}

/**
 * Sets client information to corresponding fields.
 */
void ViewUpdateClient::setValues(const QString &id)
{
    clientId = id;

    QSqlQuery query(clientsTable);

    query.prepare("SELECT DISTINCT client_type, client_fml_name, contacts, email, auto_brand, auto_model, mileage, auto_license_plate, manufacture_year, "
                  "vin_number FROM clients_table WHERE id_client = " + clientId);
    query.exec();
    query.next();

    QString clientType = query.value(0).toString();
    if ((clientType == "Физ. лицо") || (clientType == "Фіз. особа") || (clientType == "Private"))
        ui->clientTypeComboBox->setCurrentIndex(0);
    else if ((clientType == "Юр. лицо") || (clientType == "Юр. особа") || (clientType == "Organization"))
        ui->clientTypeComboBox->setCurrentIndex(1);

    ui->clientFMLname->setText(query.value(1).toString());
    QString contacts = query.value(2).toString();
    contacts.replace("\n", ", ");
    ui->contacts->setText(contacts);
    ui->email->setText(query.value(3).toString());
    ui->autoBrand->setText(query.value(4).toString());
    ui->autoModel->setText(query.value(5).toString());
    ui->mileageLine->setText(query.value(6).toString());
    ui->autoLicensePlate->setText(query.value(7).toString());
    ui->manufactureYear->setText(query.value(8).toString());
    ui->VINnumber->setText(query.value(9).toString());

    loadOrderHistoryTable();
}

/**
 * Loads order history list by client to tableView.
 */
void ViewUpdateClient::loadOrderHistoryTable()
{
    queryModel = new QSqlQueryModel(this);

    QString queryString = "SELECT id_order, order_status, creation_date, client_type, client, auto_brand, auto_model, "
                          "service_address, price FROM orders_history WHERE client = '" + ui->clientFMLname->text() + "' AND contacts LIKE '%" + ui->contacts->text() +"%'";

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

/**
 * Loads user settings from register for access granting.
 */
void ViewUpdateClient::loadUserSettings()
{
    QString userLogin = global::getSettingsValue("userLogin", "settings").toString();
    int pos = 0;

    QRegularExpression chiefRegexp("^[1][0-9]{3}$");
    QRegularExpressionValidator chiefValidator(chiefRegexp, this);

    QRegularExpression managerRegexp("^[2][0-9]{3}$");
    QRegularExpressionValidator managerValidator(managerRegexp, this);

    if ((chiefValidator.validate(userLogin, pos)) || (managerValidator.validate(userLogin, pos)))
    {
        ui->updateInfoButton->setEnabled(true);
        ui->createOrderByClientButton->setEnabled(true);
    }
}

/**
 * Allows information update.
 */
void ViewUpdateClient::on_updateInfoButton_clicked()
{
    ui->updateInfoButton->setEnabled(false);    
    ui->clientTypeComboBox->setEnabled(true);
    ui->clientFMLname->setReadOnly(false);
    ui->contacts->setReadOnly(false);
    ui->email->setReadOnly(false);
    ui->autoBrand->setReadOnly(false);
    ui->autoModel->setReadOnly(false);
    ui->autoLicensePlate->setReadOnly(false);
    ui->manufactureYear->setReadOnly(false);
    ui->autoLicensePlate->setReadOnly(false);
    ui->VINnumber->setReadOnly(false);
    ui->deleteClientButton->setEnabled(true);
    ui->saveUpdatedInfo->setEnabled(true);
}

/**
 * Opens AddOrder window with input paremeters.
 */
void ViewUpdateClient::on_createOrderByClientButton_clicked()
{
    QDialog::close();

    QString clientName = ui->clientFMLname->text();
    QString contacts = ui->contacts->text();
    QString autoBrand = ui->autoBrand->text();

    addOrder = new AddOrder;    
    connect(this, &ViewUpdateClient::sendValues, addOrder, &AddOrder::setValues);
    emit sendValues(clientName, contacts, autoBrand);
    addOrder->show();
    addOrder->setAttribute(Qt::WA_DeleteOnClose);
}

/**
 * Delets client from DB.
 */
void ViewUpdateClient::on_deleteClientButton_clicked()
{
    QSqlQuery queryRemoveClient(clientsTable);

    int msgBox = QMessageBox::information(this, tr("Предупреждение"), tr("Вы уверены, что хотите удалить клиента?"), QMessageBox::Ok, QMessageBox::Cancel);

    switch (msgBox)
    {
    case QMessageBox::Ok:

        queryRemoveClient.prepare("DELETE FROM clients_table WHERE id_client = ?");
        queryRemoveClient.addBindValue(clientId);
        queryRemoveClient.exec();

        QDialog::close();

        QMessageBox::information(this, tr("Уведомление"), tr("Клиент успешно удален!"), QMessageBox::Ok);
        break;
    case QMessageBox::Cancel:
        return;
        break;
    default:
        break;
    }
}
