#include "ViewEmployee.h"
#include "ui_ViewEmployee.h"

ViewEmployee::ViewEmployee(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ViewEmployee)
{
    ui->setupUi(this);

    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowFlags(windowFlags() & Qt::WindowMinimizeButtonHint);
}

ViewEmployee::~ViewEmployee()
{
    delete ui;
}

void ViewEmployee::closeEvent(QCloseEvent*)
{
    QDialog::close();

    listEmployees = new ListEmployees;
    listEmployees->show();
    listEmployees->setAttribute(Qt::WA_DeleteOnClose);
}

// Gets employee's id from ListEmployee
void ViewEmployee::setValues(const QString &id)
{
    employeeId = id;

    QSqlQuery query(employeesDB);

    query.prepare("SELECT DISTINCT employee_FML_name, employee_position, hour_payment, service_address "
        "FROM EmployeesTable WHERE id_employee = " + employeeId);

    query.exec();
    query.next();

    ui->workerFMLname->setText(query.value(0).toString());
    ui->workerPosition->setText(query.value(1).toString());
    ui->hourlyPayment->setText(query.value(2).toString());
    ui->serviceNumber->setText(query.value(3).toString());
}

void ViewEmployee::on_updateEmployeeInfoButton_clicked()
{
    QDialog::hide();

    updateEmployee = new UpdateEmployee;
    updateEmployee->setValues(employeeId);
    connect(updateEmployee, &UpdateEmployee::sendData, this, &ViewEmployee::receiveData);
    updateEmployee->show();
    updateEmployee->setAttribute(Qt::WA_DeleteOnClose);
}

void ViewEmployee::receiveData(bool update)
{
    if (update)
        QDialog::close();
    else
        QDialog::show();
}
