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

/**
 * Event on window close.
 */
void ViewUpdateEmployee::closeEvent(QCloseEvent *)
{
    QDialog::close();

    listEmployees = new ListEmployees;
    listEmployees->show();
    listEmployees->setAttribute(Qt::WA_DeleteOnClose);
}

/**
 * Sets employee information to corresponding fields.
 */
void ViewUpdateEmployee::setValues(const QString &id)
{
    employeeId = id;

    QSqlQuery query(employeesTable);
    query.prepare("SELECT DISTINCT employee_fml_name, employee_position, hour_payment, service_address FROM employees_table WHERE id_employee = " + employeeId);
    query.exec();
    query.next();

    ui->workerFMLname->setText(query.value(0).toString());
    ui->workerPosition->setText(query.value(1).toString());
    ui->hourlyPayment->setText(query.value(2).toString());
    ui->serviceNumber->setText(query.value(3).toString());
}

/**
 * Checks input information and updates employee in DB.
 */
void ViewUpdateEmployee::on_saveUpdatedInfo_clicked()
{
    QSqlQuery query(employeesTable);

    QString employeeFMLname = ui->workerFMLname->text();
    QString employeePosition = ui->workerPosition->text();
    QString hourlyPayment = ui->hourlyPayment->text();
    QString serviceNumber = ui->serviceNumber->text();

    if ((employeeFMLname.isEmpty()) || (employeePosition.isEmpty()) || (hourlyPayment.isEmpty()) || (serviceNumber.isEmpty()))
    {
        ui->errorLabel->show();
        return;
    }

    query.prepare("UPDATE employees_table SET employee_fml_name = ?, employee_position = ?, hour_payment = ?, service_address = ? WHERE id_employee = ?");
    query.addBindValue(employeeFMLname);
    query.addBindValue(employeePosition);
    query.addBindValue(hourlyPayment);
    query.addBindValue(serviceNumber);
    query.addBindValue(employeeId);
    query.exec();

    QDialog::close();

    QMessageBox::information(this, tr("Уведомление"), tr("Информация о сотруднике успешно обновлена!"), QMessageBox::Ok);
}

/**
 * Allows information update.
 */
void ViewUpdateEmployee::on_updateEmployeeInfoButton_clicked()
{
    ui->updateEmployeeInfoButton->setEnabled(false);
    ui->workerFMLname->setReadOnly(false);
    ui->workerPosition->setReadOnly(false);
    ui->serviceNumber->setReadOnly(false);
    ui->hourlyPayment->setReadOnly(false);
    ui->deleteEmployeeButton->setEnabled(true);
    ui->saveUpdatedInfo->setEnabled(true);
}

/**
 * Delets employee from DB.
 */
void ViewUpdateEmployee::on_deleteEmployeeButton_clicked()
{
    QSqlQuery queryRemoveEmployee(employeesTable);

    int msgBox = QMessageBox::information(this, tr("Предупреждение"), tr("Вы уверены, что хотите удалить сотрудника?"), QMessageBox::Ok, QMessageBox::Cancel);

    switch (msgBox)
    {
    case QMessageBox::Ok:

        queryRemoveEmployee.prepare("DELETE FROM employees_table WHERE id_employee = ?");
        queryRemoveEmployee.addBindValue(employeeId);
        queryRemoveEmployee.exec();

        QDialog::close();

        QMessageBox::information(this, tr("Уведомление"), tr("Сотрудник успешно удален!"), QMessageBox::Ok);
        break;
    case QMessageBox::Cancel:
        return;
        break;
    default:
        break;
    }
}
