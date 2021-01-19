#include "UpdateEmployee.h"
#include "ui_UpdateEmployee.h"

UpdateEmployee::UpdateEmployee(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UpdateEmployee)
{
    ui->setupUi(this);

    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowFlags(windowFlags() & Qt::WindowMinimizeButtonHint);

    ui->errorLabel->hide();
}

UpdateEmployee::~UpdateEmployee()
{
    delete ui;
}

void UpdateEmployee::setValues(const QString &id)
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

void UpdateEmployee::on_backToViewInfoButton_clicked()
{
    emit sendData(false);

    QDialog::close();
}

void UpdateEmployee::on_saveUpdatedInfo_clicked()
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
