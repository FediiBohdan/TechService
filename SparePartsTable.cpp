#include "SparePartsTable.h"
#include "ui_SparePartsTable.h"

SparePartsTable::SparePartsTable(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SparePartsTable)
{
    ui->setupUi(this);

    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowFlags(windowFlags() & Qt::WindowMinimizeButtonHint);

    ui->tableView->verticalHeader()->setSectionsClickable(false);
    ui->tableView->horizontalHeader()->setSectionsClickable(false);

    //connect(startWindow, &StartWindow::closeAllWindowsExceptCurrent, this, &SparePartsTable::closeWindow);

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("D:\\Diploma\\CRM_AutoService\\ServiceStationDB.db");
    db.open();

    loadTable();
}

SparePartsTable::~SparePartsTable()
{
    delete ui;
}

void SparePartsTable::loadTable()
{
    queryModel = new QSqlQueryModel(this);

    QString queryString;

    queryString = "SELECT id_spare_part, spare_name, manufacturer, quantity_in_stock, auto_compatibility, original, price FROM SparePartsCatalogue";

    queryModel->setQuery(queryString, sparePartsTable);

    queryModel->setHeaderData(0, Qt::Horizontal, tr("id"));
    queryModel->setHeaderData(1, Qt::Horizontal, tr("Название"));
    queryModel->setHeaderData(2, Qt::Horizontal, tr("Производитель"));
    queryModel->setHeaderData(3, Qt::Horizontal, tr("Количество в наличии"));
    queryModel->setHeaderData(4, Qt::Horizontal, tr("Совместимость с моделями"));
    queryModel->setHeaderData(5, Qt::Horizontal, tr("Оригинал"));
    queryModel->setHeaderData(6, Qt::Horizontal, tr("Цена"));

    ui->tableView->setModel(queryModel);

    for (qint32 row_index = 0; row_index < ui->tableView->model()->rowCount(); ++row_index)
    {
        QString idSparePart = queryModel->data(queryModel->index(row_index, 0)).toString();
        QString spareName = queryModel->data(queryModel->index(row_index, 1)).toString();
        QString manufacturer = queryModel->data(queryModel->index(row_index, 2)).toString();
        QString quantityInStock = queryModel->data(queryModel->index(row_index, 3)).toString();
        QString autoCompatibility = queryModel->data(queryModel->index(row_index, 4)).toString();
        QString original = queryModel->data(queryModel->index(row_index, 5)).toString();
        QString price = queryModel->data(queryModel->index(row_index, 6)).toString();

        QSqlQuery query(db);
    }

    ui->tableView->horizontalHeader()->setDefaultSectionSize(maximumWidth());
    ui->tableView->resizeColumnsToContents();
}

void SparePartsTable::closeWindow()
{
    close();
}
