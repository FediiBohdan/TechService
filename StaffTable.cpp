#include "StaffTable.h"
#include "ui_StaffTable.h"

#include <QMessageBox>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QDebug>

#include <QStandardItemModel>
#include <QStandardItem>

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
//    queryModel->setHeaderData(2, Qt::Horizontal, tr("Кому"));
//    queryModel->insertColumn(4);
//    queryModel->setHeaderData(4, Qt::Horizontal, tr("Статус"));
//    queryModel->setHeaderData(5, Qt::Horizontal, tr("Дата и время"));
//    queryModel->insertColumn(6);
//    queryModel->setHeaderData(6, Qt::Horizontal, tr("Заметка"));

    ui->tableView->setModel(queryModel);

//    if (ui->tabWidget->currentIndex() == 3)
//        ui->tableView->setColumnHidden(1, true);

//    ui->tableView->setColumnHidden(3, true);

//    if (ui->tabWidget->currentIndex() == 1 || ui->tabWidget->currentIndex() == 2)
//        ui->tableView->setColumnHidden(4, true);

//    ui->tableView->setColumnHidden(7, true);
//    ui->tableView->setColumnHidden(8, true);

    for (qint32 row_index = 0; row_index < ui->tableView->model()->rowCount(); ++row_index)
    {
        QString id_staff = queryModel->data(queryModel->index(row_index, 0)).toString();
        QString staffPersonalInfo = queryModel->data(queryModel->index(row_index, 1)).toString();
        QString staffPosition = queryModel->data(queryModel->index(row_index, 2)).toString();
//        QString dst = queryModel->data(queryModel->index(row_index, 2)).toString();
//        QString dialogStatus = queryModel->data(queryModel->index(row_index, 3)).toString();
//        QString uniqueid = queryModel->data(queryModel->index(row_index, 7)).toString();

//        if (extfield.isEmpty())
//            ui->tableView->setIndexWidget(queryModel->index(row_index, 0), loadName(src, dst));

//        if (ui->tabWidget->currentIndex() == 0 || ui->tabWidget->currentIndex() == 3)
//            ui->tableView->setIndexWidget(queryModel->index(row_index, 4), loadStatus(dialogStatus));

        QSqlQuery query(db);

//        query.prepare("SELECT EXISTS(SELECT note FROM calls WHERE uniqueid = "+uniqueid+")");
//        query.exec();
//        query.first();

//        if (query.value(0).toBool())
//        {
//            ui->tableView->setIndexWidget(queryModel->index(row_index, 6), loadNote(uniqueid));

//            ui->tableView->resizeRowToContents(row_index);
//        }
//        else
//            ui->tableView->setRowHeight(row_index, 34);
    }

    ui->tableView->horizontalHeader()->setDefaultSectionSize(maximumWidth());

    ui->tableView->resizeColumnsToContents();

//    if (ui->tableView->model()->columnCount() != 0)
//        ui->tableView->horizontalHeader()->setSectionResizeMode(6, QHeaderView::Stretch);

//    if (!selections.isEmpty())
//        for (qint32 i = 0; i < selections.length(); ++i)
//        {
//            QModelIndex index = selections.at(i);

//            ui->tableView->selectionModel()->select(index, QItemSelectionModel::Select | QItemSelectionModel::Rows);
//        }
//    else
//        disableButtons();
}
