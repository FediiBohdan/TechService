#include "AddEmployee.h"
#include "ui_AddEmployee.h"

AddEmployee::AddEmployee(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddEmployee)
{
    ui->setupUi(this);

    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowFlags(windowFlags() & Qt::WindowMinimizeButtonHint);

    employeeDB = QSqlDatabase::addDatabase("QSQLITE");
    employeeDB.setDatabaseName("C:\\Users\\BohdanF\\Documents\\Diploma\\CRM_AutoService\\ServiceStationDB.db");
    employeeDB.open();
}

AddEmployee::~AddEmployee()
{
    delete ui;
}

void AddEmployee::on_saveWorkerButton_clicked()
{
    QSqlQuery query(employeeDB);

    QString employeeFMLname = ui->workerFMLname->text();
    QString employeePosition = ui->workerPosition->text();
    QString hourlyPayment = ui->hourlyPayment->text();
    QString serviceNumber = ui->serviceNumber->text();

    query.prepare("INSERT INTO ListEmployees (employee_FML_name, employee_position, hour_payment, service_number)"
        "VALUES(?, ?, ?, ?)");

    query.addBindValue(employeeFMLname);
    query.addBindValue(employeePosition);
    query.addBindValue(hourlyPayment);
    query.addBindValue(serviceNumber);
    query.exec();

    QDialog::close();

    QMessageBox::information(this, tr("Уведомление"), tr("Сотрудник успешно добавлен!"), QMessageBox::Ok);
}
