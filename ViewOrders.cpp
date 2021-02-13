#include "ViewOrders.h"
#include "ui_ViewOrders.h"

ViewOrders::ViewOrders(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ViewOrders)
{
    ui->setupUi(this);

    QDialog::showMaximized();

    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowFlags(windowFlags() & Qt::WindowMinimizeButtonHint);
}

ViewOrders::~ViewOrders()
{
    delete ui;
}

void ViewOrders::closeEvent(QCloseEvent*)
{
    QDialog::close();

    listOrders = new ListOrders;
    listOrders->show();
    listOrders->setAttribute(Qt::WA_DeleteOnClose);
}

// Gets spare part's id from ListSpareParts
void ViewOrders::setValues(const QString &id)
{
    orderId = id;

    QSqlQuery query(ordersDB);

    query.prepare("SELECT DISTINCT client_type, client, creation_date, creation_time, updating_date, updating_time, contacts, email, auto_brand, "
                  "auto_model, manufacture_year, VIN_number, auto_license_plate, service_address, discounts, order_status, "
                  "spare_parts_list, works_list, feedback FROM OrdersHistory WHERE id_order = " + orderId);

    query.exec();
    query.next();

    ui->clientTypeLine->setText(query.value(0).toString());
    ui->clientLine->setText(query.value(1).toString());
    ui->dateLine->setText(query.value(2).toString());
    ui->timeLine->setText(query.value(3).toString());
    ui->updateDateLine->setText(query.value(4).toString());
    ui->updateTimeLine->setText(query.value(5).toString());
    ui->contactLine->setText(query.value(6).toString());
    ui->emailLine->setText(query.value(7).toString());
    ui->brandLine->setText(query.value(8).toString());
    ui->modelLine->setText(query.value(9).toString());
    ui->yearLine->setText(query.value(10).toString());
    ui->VIN_Line->setText(query.value(11).toString());
    ui->autoLicensePlateLine->setText(query.value(12).toString());
    ui->serviceLine->setText(query.value(13).toString());
    ui->discountsLine->setText(query.value(14).toString());
    ui->orderStatusLine->setText(query.value(15).toString());
    ui->sparePartsList->setText(query.value(16).toString());
    ui->worksList->setText(query.value(17).toString());
    ui->feedback->setText(query.value(18).toString());

    QSqlQuery queryEmployee(orderDetailDB);

    queryEmployee.prepare("SELECT order_employee, employee_work_hours, employee_position "
                          "FROM OrderDetailTable WHERE id_order = " + orderId);
    queryEmployee.exec();

    while (queryEmployee.next())
    {
        QString employeePosition = queryEmployee.value(2).toString();

        if (employeePosition == "Механик" && ui->mechanicLine->text().isEmpty())
        {
            ui->mechanicLine->setText(queryEmployee.value(0).toString());
            ui->mechanicHoursLine->setText(queryEmployee.value(1).toString());
        }

        else if (employeePosition == "Механик_2" && !ui->mechanicLine->text().isEmpty())
        {
            ui->mechanic2Line->setText(queryEmployee.value(0).toString());
            ui->mechanic2HoursLine->setText(queryEmployee.value(1).toString());
        }

        else if (employeePosition == "Диагност")
        {
            ui->diagnosticianLine->setText(queryEmployee.value(0).toString());
            ui->diagnosticianHoursLine->setText(queryEmployee.value(1).toString());
        }

        else if (employeePosition == "Электронщик")
        {
            ui->electronicsLine->setText(queryEmployee.value(0).toString());
            ui->electronicsHoursLine->setText(queryEmployee.value(1).toString());
        }

        else if (employeePosition == "Слесарь")
        {
            ui->locksmithLine->setText(queryEmployee.value(0).toString());
            ui->locksmithHoursLine->setText(queryEmployee.value(1).toString());
        }

        else if (employeePosition == "Мойщик")
        {
            ui->washerLine->setText(queryEmployee.value(0).toString());
            ui->washerHoursLine->setText(queryEmployee.value(1).toString());
        }
    }
}

void ViewOrders::on_updateOrderInfoButton_clicked()
{
    QDialog::hide();

    updateOrders = new UpdateOrders;
    updateOrders->setValues(orderId);
    connect(updateOrders, &UpdateOrders::sendData, this, &ViewOrders::receiveData);
    updateOrders->show();
    updateOrders->setAttribute(Qt::WA_DeleteOnClose);
}

void ViewOrders::receiveData(bool update)
{
    if (update)
        QDialog::close();
    else
        QDialog::show();
}
