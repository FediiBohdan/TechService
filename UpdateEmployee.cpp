#include "UpdateEmployee.h"
#include "ui_UpdateEmployee.h"

UpdateEmployee::UpdateEmployee(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UpdateEmployee)
{
    ui->setupUi(this);

    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowFlags(windowFlags() & Qt::WindowMinimizeButtonHint);
}

UpdateEmployee::~UpdateEmployee()
{
    delete ui;
}

void UpdateEmployee::setValues(const QString& id)
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

    query.prepare("UPDATE EmployeesTable SET employee_FML_name = ?, employee_position = ?, hour_payment = ?, service_number = ? WHERE id_employee = ?");

    query.addBindValue(employeeFMLname);
    query.addBindValue(employeePosition);
    query.addBindValue(hourlyPayment);
    query.addBindValue(serviceNumber);
    query.addBindValue(employeeId);
    query.exec();

    QDialog::close();

    QMessageBox::information(this, tr("Уведомление"), tr("Сотрудник успешно добавлен!"), QMessageBox::Ok);
}
