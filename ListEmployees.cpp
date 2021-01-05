#include "ListEmployees.h"
#include "ui_ListEmployees.h"

#include <QMessageBox>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QDebug>
#include <QAbstractItemView>

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

    connect(ui->tableView, &QAbstractItemView::doubleClicked, this, &ListEmployees::showEmployeeInfo);

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

    queryString = "SELECT id_employee, employee_FML_name, employee_position, hour_payment, service_number FROM EmployeesTable ORDER BY service_number";

    queryModel->setQuery(queryString, employeesTable);

    queryModel->setHeaderData(0, Qt::Horizontal, tr("id"));
    queryModel->setHeaderData(1, Qt::Horizontal, tr("ФИО работников"));
    queryModel->setHeaderData(2, Qt::Horizontal, tr("Должность"));
    queryModel->setHeaderData(3, Qt::Horizontal, tr("Почасовая оплата"));
    queryModel->setHeaderData(4, Qt::Horizontal, tr("Место работы"));

    ui->tableView->setModel(queryModel);

    for (qint32 row_index = 0; row_index < ui->tableView->model()->rowCount(); ++row_index)
    {
        QString workPlace = queryModel->data(queryModel->index(row_index, 4)).toString();

        // needed replace int by QString from DB
        QSqlQuery query(employeesDB);
    }

    ui->tableView->setColumnHidden(0, true);

    ui->tableView->horizontalHeader()->setDefaultSectionSize(maximumWidth());
    ui->tableView->resizeColumnsToContents();
}

void ListEmployees::on_addWorkerButton_clicked()
{
    addEmployee = new AddEmployee();
    addEmployee->show();
    addEmployee->setAttribute(Qt::WA_DeleteOnClose);
}

void ListEmployees::showEmployeeInfo(const QModelIndex &index)
{
    QString employeeId = queryModel->data(queryModel->index(index.row(), 0)).toString();

    viewEmployees = new ViewEmployee;
    viewEmployees->setValues(employeeId);
    viewEmployees->show();
    viewEmployees->setAttribute(Qt::WA_DeleteOnClose);
}
