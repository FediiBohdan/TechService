#include "AddEmployee.h"
#include "ui_AddEmployee.h"

AddEmployee::AddEmployee(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddEmployee)
{
    ui->setupUi(this);

    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowFlags(windowFlags() & Qt::WindowMinimizeButtonHint);

    QDir tempDirDB = QDir::currentPath(); tempDirDB.cdUp(); QString dirDB = tempDirDB.path();

    employeeDB = QSqlDatabase::addDatabase("QSQLITE");
    employeeDB.setDatabaseName(dirDB + "\\CRM_AutoService\\ServiceStationDB.db");
    employeeDB.open();

    ui->serviceAddress->addItems(QStringList() << "Street A, 123" << "Street B, 456" << "Street C, 789");

    ui->errorLabel->hide();
}

AddEmployee::~AddEmployee()
{
    delete ui;
}

void AddEmployee::closeEvent(QCloseEvent*)
{
    QDialog::close();

    listEmployees = new ListEmployees;
    listEmployees->show();
    listEmployees->setAttribute(Qt::WA_DeleteOnClose);
}

void AddEmployee::on_saveWorkerButton_clicked()
{
    QSqlQuery query(employeeDB);

    QString employeeFMLname = ui->workerFMLname->text();
    QString employeePosition = ui->workerPosition->text();
    QString hourlyPayment = ui->hourlyPayment->text();
    QString serviceAddress = ui->serviceAddress->currentText();

    if (employeeFMLname.isEmpty() || employeePosition.isEmpty() || hourlyPayment.isEmpty())
    {
        ui->errorLabel->show();
        return;
    }

    query.prepare("INSERT INTO EmployeesTable (employee_FML_name, employee_position, hour_payment, service_address)"
        "VALUES(?, ?, ?, ?)");

    query.addBindValue(employeeFMLname);
    query.addBindValue(employeePosition);
    query.addBindValue(hourlyPayment);
    query.addBindValue(serviceAddress);
    query.exec();

    QDialog::close();

    QMessageBox::information(this, tr("Уведомление"), tr("Сотрудник успешно добавлен!"), QMessageBox::Ok);
}
