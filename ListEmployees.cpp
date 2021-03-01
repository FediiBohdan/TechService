#include "ListEmployees.h"
#include "ui_ListEmployees.h"

#include <QMessageBox>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QDebug>
#include <QAbstractItemView>
#include <QTableWidgetItem>

ListEmployees::ListEmployees(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ListEmployees)
{
    ui->setupUi(this);

    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowFlags(windowFlags() & Qt::WindowMinimizeButtonHint);

    QDialog::showMaximized();

    ui->tableView->verticalHeader()->setSectionsClickable(false);
    ui->tableView->horizontalHeader()->setSectionsClickable(false);

    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);

    QDir tempDirDB = QDir::currentPath(); tempDirDB.cdUp(); QString dirDB = tempDirDB.path();

    employeesDB = QSqlDatabase::addDatabase("QSQLITE");
    employeesDB.setDatabaseName(dirDB + "\\CRM_AutoService\\ServiceStationDB.db");
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

    QString queryString = "SELECT id_employee, employee_FML_name, employee_position, hour_payment, service_address FROM EmployeesTable";

    queryModel->setQuery(queryString, employeesTable);

    queryModel->setHeaderData(0, Qt::Horizontal, tr("id"));
    queryModel->setHeaderData(1, Qt::Horizontal, tr("ФИО работников"));
    queryModel->setHeaderData(2, Qt::Horizontal, tr("Должность"));
    queryModel->setHeaderData(3, Qt::Horizontal, tr("Почасовая оплата"));
    queryModel->setHeaderData(4, Qt::Horizontal, tr("Адрес СТО"));

    ui->tableView->setModel(queryModel);

    ui->tableView->setColumnHidden(0, true);

    ui->tableView->horizontalHeader()->setDefaultSectionSize(maximumWidth());
    ui->tableView->resizeColumnsToContents();
}

void ListEmployees::on_addWorkerButton_clicked()
{
    QDialog::close();

    addEmployee = new AddEmployee();
    addEmployee->show();
    addEmployee->setAttribute(Qt::WA_DeleteOnClose);
}

void ListEmployees::showEmployeeInfo(const QModelIndex &index)
{
    QDialog::close();

    QString employeeId = queryModel->data(queryModel->index(index.row(), 0)).toString();

    viewEmployees = new ViewEmployee;
    viewEmployees->setValues(employeeId);
    viewEmployees->show();
    viewEmployees->setAttribute(Qt::WA_DeleteOnClose);
}

void ListEmployees::on_updateButton_clicked()
{
    queryModel->setQuery(NULL);

    loadTable();
}

void ListEmployees::saveAsCSV(QString filename)
{
    QFile csvFile (filename);

    if (csvFile.open(QIODevice::WriteOnly))
    {
        QTextStream textStream(&csvFile);
        QStringList stringList;

        stringList << "\" \"";
        for (int column = 1; column < ui->tableView->horizontalHeader()->count(); ++column)
            stringList << "\"" + ui->tableView->model()->headerData(column, Qt::Horizontal).toString() + "\"";

        textStream << stringList.join(";") + "\n";

        for (int row = 0; row < ui->tableView->verticalHeader()->count(); ++row)
        {
            stringList.clear();
            stringList << "\"" + ui->tableView->model()->headerData(row, Qt::Vertical).toString() + "\"";

            for (int column = 1; column < ui->tableView->horizontalHeader()->count(); ++column)
                stringList << "\"" + ui->tableView->model()->data(ui->tableView->model()->index(row, column), Qt::DisplayRole).toString() + "\"";

            textStream << stringList.join(";") + "\n";
        }

        csvFile.close();

        QMessageBox::information(this, tr("Уведомление"), tr("База сотрудников успешно экспортирована!"), QMessageBox::Ok);
    }
}

void ListEmployees::on_csvExportButton_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Экспорт"), "", "CSV (*.csv);;All Files (*)");
    saveAsCSV(fileName);
}
