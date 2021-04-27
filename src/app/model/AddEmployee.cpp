#include "AddEmployee.h"
#include "ui_AddEmployee.h"

AddEmployee::AddEmployee(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddEmployee)
{
    ui->setupUi(this);

    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowFlags(windowFlags() & Qt::WindowMinimizeButtonHint);

    ui->serviceAddress->addItems(QStringList() << "Среднефонтанская, 30А (Приморский р-н)" << "Платонова, 56 (Малиновский р-н)" << "Архитекторская, 28 (Киевский р-н)");

    ui->errorLabel->hide();
}

AddEmployee::~AddEmployee()
{
    delete ui;
}

/**
 * Event on window close.
 */
void AddEmployee::closeEvent(QCloseEvent *)
{
    QDialog::close();

    listEmployees = new ListEmployees;
    listEmployees->show();
    listEmployees->setAttribute(Qt::WA_DeleteOnClose);
}

/**
 * Checks input information and saves new employee to DB.
 */
void AddEmployee::on_saveWorkerButton_clicked()
{
    QSqlQuery query(employeeTable);

    QString employeeFMLname = ui->workerFMLname->text();
    QString employeePosition = ui->workerPosition->text();
    QString hourlyPayment = ui->hourlyPayment->text();
    QString serviceAddress = ui->serviceAddress->currentText();

    if (employeeFMLname.isEmpty() || employeePosition.isEmpty() || hourlyPayment.isEmpty())
    {
        ui->errorLabel->show();
        return;
    }

    query.prepare("INSERT INTO employees_table (employee_fml_name, employee_position, hour_payment, service_address) VALUES(?, ?, ?, ?)");
    query.addBindValue(employeeFMLname);
    query.addBindValue(employeePosition);
    query.addBindValue(hourlyPayment);
    query.addBindValue(serviceAddress);
    query.exec();

    QDialog::close();

    QMessageBox::information(this, tr("Уведомление"), tr("Сотрудник успешно добавлен!"), QMessageBox::Ok);
}
