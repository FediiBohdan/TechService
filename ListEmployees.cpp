#include "ListEmployees.h"
#include "ui_ListEmployees.h"

#include <QMessageBox>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QDebug>

ListEmployees::ListEmployees(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ListEmployees)
{
    ui->setupUi(this);

    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowFlags(windowFlags() & Qt::WindowMinimizeButtonHint);

    ui->tableView->verticalHeader()->setSectionsClickable(false);
    ui->tableView->horizontalHeader()->setSectionsClickable(false);

    employeesDB = QSqlDatabase::addDatabase("QSQLITE");
    employeesDB.setDatabaseName("D:\\Diploma\\CRM_AutoService\\ServiceStationDB.db");
    employeesDB.open();

    loadTable();
}

ListEmployees::~ListEmployees()
{
    delete ui;
}

void ListEmployees::loadTable()
{
    queryModel = new QSqlQueryModel(this);

    QString queryString;

    queryString = "SELECT id_employee, employee_FML_name, employee_position, service_number FROM EmployeesTable";

    queryModel->setQuery(queryString, employeesTable);

    queryModel->setHeaderData(0, Qt::Horizontal, tr("id"));
    queryModel->setHeaderData(1, Qt::Horizontal, tr("ФИО работников"));
    queryModel->setHeaderData(2, Qt::Horizontal, tr("Должность"));
    queryModel->setHeaderData(3, Qt::Horizontal, tr("Место работы"));

    ui->tableView->setModel(queryModel);

    for (qint32 row_index = 0; row_index < ui->tableView->model()->rowCount(); ++row_index)
    {
        QString id_employee = queryModel->data(queryModel->index(row_index, 0)).toString();
        QString employeePersonalInfo = queryModel->data(queryModel->index(row_index, 1)).toString();
        QString employeePosition = queryModel->data(queryModel->index(row_index, 2)).toString();
        QString workPlace = queryModel->data(queryModel->index(row_index, 3)).toString();

        QSqlQuery query(employeesDB);
    }

    ui->tableView->horizontalHeader()->setDefaultSectionSize(maximumWidth());
    ui->tableView->resizeColumnsToContents();
}

void ListEmployees::on_addWorkerButton_clicked()
{
    addEmployee = new AddEmployee();
    addEmployee->show();
    addEmployee->setAttribute(Qt::WA_DeleteOnClose);
}
