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
    QDialog::hide();

    listEmployees = new ListEmployees;
    listEmployees->show();
    listEmployees->setAttribute(Qt::WA_DeleteOnClose);
}

// Gets employee's id from ListEmployee
void ViewEmployee::setValues(const QString &id)
{
    employeeId = id;

    QSqlQuery query(employeesDB);

    query.prepare("SELECT DISTINCT employee_FML_name, employee_position, hour_payment, service_number "
                  "FROM EmployeesTable WHERE id_employee = " + employeeId);
    query.exec();
    query.next();

    ui->workerFMLname->setText(query.value(0).toString());
    ui->workerPosition->setText(query.value(1).toString());
    ui->hourlyPayment->setText(query.value(2).toString());

    QString serviceAddress = query.value(3).toString();
        if (serviceAddress == '1')
        {
            ui->serviceNumber->setText("ул. XXX, 76");
        }
        else if (serviceAddress == '2')
        {
            ui->serviceNumber->setText("ул. YYY, 34");
        }
        else if (serviceAddress == '3')
        {
            ui->serviceNumber->setText("ул. ZZZ, 2");
        }
}
