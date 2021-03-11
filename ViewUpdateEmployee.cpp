#include "ViewUpdateEmployee.h"
#include "ui_ViewUpdateEmployee.h"

ViewUpdateEmployee::ViewUpdateEmployee(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ViewUpdateEmployee)
{
    ui->setupUi(this);

    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowFlags(windowFlags() & Qt::WindowMinimizeButtonHint);

    ui->errorLabel->hide();
}

ViewUpdateEmployee::~ViewUpdateEmployee()
{
    delete ui;
}

void ViewUpdateEmployee::setValues(const QString &id)
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

void ViewUpdateEmployee::on_saveUpdatedInfo_clicked()
{
    QSqlQuery query(employeesDB);

    QString employeeFMLname = ui->workerFMLname->text();
    QString employeePosition = ui->workerPosition->text();
    QString hourlyPayment = ui->hourlyPayment->text();
    QString serviceNumber = ui->serviceNumber->text();

    if (employeeFMLname.isEmpty() || employeePosition.isEmpty() || hourlyPayment.isEmpty() || serviceNumber.isEmpty())
    {
        ui->errorLabel->show();
        return;
    }

    query.prepare("UPDATE EmployeesTable SET employee_FML_name = ?, employee_position = ?, hour_payment = ?, service_address = ? WHERE id_employee = ?");

    query.addBindValue(employeeFMLname);
    query.addBindValue(employeePosition);
    query.addBindValue(hourlyPayment);
    query.addBindValue(serviceNumber);
    query.addBindValue(employeeId);
    query.exec();

    QDialog::close();

    QMessageBox::information(this, tr("Уведомление"), tr("Информация о сотруднике успешно обновлена!"), QMessageBox::Ok);
}

void ViewUpdateEmployee::on_updateEmployeeInfoButton_clicked()
{
    ui->updateEmployeeInfoButton->setEnabled(false);
    ui->workerFMLname->setReadOnly(false);
    ui->workerPosition->setReadOnly(false);
    ui->serviceNumber->setReadOnly(false);
    ui->hourlyPayment->setReadOnly(false);
    ui->saveUpdatedInfo->setEnabled(true);
}
