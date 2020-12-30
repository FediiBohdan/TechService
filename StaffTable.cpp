#include "StaffTable.h"
#include "ui_StaffTable.h"

#include <QMessageBox>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QDebug>

StaffTable::StaffTable(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StaffTable)
{
    ui->setupUi(this);

    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowFlags(windowFlags() & Qt::WindowMinimizeButtonHint);

    ui->tableView->verticalHeader()->setSectionsClickable(false);
    ui->tableView->horizontalHeader()->setSectionsClickable(false);

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("D:\\Diploma\\CRM_AutoService\\ServiceStationDB.db");
    db.open();

    loadTable();
}

StaffTable::~StaffTable()
{
    delete ui;
}

void StaffTable::loadTable()
{
    queryModel = new QSqlQueryModel(this);

    QString queryString;

    queryString = "SELECT id_staff, staff_FML_name, staff_position, service_number FROM StaffTable";

    queryModel->setQuery(queryString, staffTable);

    queryModel->setHeaderData(0, Qt::Horizontal, tr("id"));
    queryModel->setHeaderData(1, Qt::Horizontal, tr("ФИО работников"));
    queryModel->setHeaderData(2, Qt::Horizontal, tr("Должность"));
    queryModel->setHeaderData(3, Qt::Horizontal, tr("Место работы"));

    ui->tableView->setModel(queryModel);

    for (qint32 row_index = 0; row_index < ui->tableView->model()->rowCount(); ++row_index)
    {
        QString id_staff = queryModel->data(queryModel->index(row_index, 0)).toString();
        QString employeePersonalInfo = queryModel->data(queryModel->index(row_index, 1)).toString();
        QString employeePosition = queryModel->data(queryModel->index(row_index, 2)).toString();
        QString workPlace = queryModel->data(queryModel->index(row_index, 3)).toString();

        QSqlQuery query(db);
    }

    ui->tableView->horizontalHeader()->setDefaultSectionSize(maximumWidth());
    ui->tableView->resizeColumnsToContents();
}

void StaffTable::on_addWorkerButton_clicked()
{
    addWorker = new AddWorker();
    addWorker->show();
    addWorker->setAttribute(Qt::WA_DeleteOnClose);
}
