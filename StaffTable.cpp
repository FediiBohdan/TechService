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

    ui->tableView->verticalHeader()->setSectionsClickable(false);
    ui->tableView->horizontalHeader()->setSectionsClickable(false);

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("D:\\Diploma\\CRM_AutoService\\ServiceStationDB.db");
    db.open();
}

StaffTable::~StaffTable()
{
    delete ui;
}

void StaffTable::loadTable()
{
    queryModel = new QSqlQueryModel(this);

    QString queryString;

    queryString = "SELECT id_staff, staff_FML_name, staff_position FROM StaffTable";

    queryModel->setQuery(queryString, staffTable);

    queryModel->setHeaderData(0, Qt::Horizontal, tr("id"));
    queryModel->setHeaderData(1, Qt::Horizontal, tr("ФИО работников"));
    queryModel->setHeaderData(2, Qt::Horizontal, tr("Должность"));

    ui->tableView->setModel(queryModel);

    for (qint32 row_index = 0; row_index < ui->tableView->model()->rowCount(); ++row_index)
    {
        QString id_staff = queryModel->data(queryModel->index(row_index, 0)).toString();
        QString staffPersonalInfo = queryModel->data(queryModel->index(row_index, 1)).toString();
        QString staffPosition = queryModel->data(queryModel->index(row_index, 2)).toString();

        QSqlQuery query(db);
    }

    ui->tableView->horizontalHeader()->setDefaultSectionSize(maximumWidth());
    ui->tableView->resizeColumnsToContents();
}
