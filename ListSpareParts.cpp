#include "ListSpareParts.h"
#include "ui_ListSpareParts.h"

ListSparePart::ListSparePart(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ListSparePart)
{
    ui->setupUi(this);

    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowFlags(windowFlags() & Qt::WindowMinimizeButtonHint);

    ui->tableView->verticalHeader()->setSectionsClickable(false);
    ui->tableView->horizontalHeader()->setSectionsClickable(false);

    //connect(startWindow, &StartWindow::closeAllWindowsExceptCurrent, this, &ListSparePart::closeWindow);

    listSparePartsDB = QSqlDatabase::addDatabase("QSQLITE");
    listSparePartsDB.setDatabaseName("D:\\Diploma\\CRM_AutoService\\ServiceStationDB.db");
    listSparePartsDB.open();

    connect(ui->tableView, &QAbstractItemView::doubleClicked, this, &ListSparePart::showSparePartInfo);

    loadTable();
}

ListSparePart::~ListSparePart()
{
    delete ui;
}

void ListSparePart::loadTable()
{
    queryModel = new QSqlQueryModel(this);

    QString queryString;

    queryString = "SELECT id_spare_part, spare_name, manufacturer, quantity_in_stock, auto_compatibility, original, "
                  "price FROM SparePartsCatalogue";

    queryModel->setQuery(queryString, listSpareParts);

    queryModel->setHeaderData(0, Qt::Horizontal, tr("id"));
    queryModel->setHeaderData(1, Qt::Horizontal, tr("Название"));
    queryModel->setHeaderData(2, Qt::Horizontal, tr("Производитель"));
    queryModel->setHeaderData(3, Qt::Horizontal, tr("Количество в наличии"));
    queryModel->setHeaderData(4, Qt::Horizontal, tr("Совместимость с моделями"));
    queryModel->setHeaderData(5, Qt::Horizontal, tr("Оригинал"));
    queryModel->setHeaderData(6, Qt::Horizontal, tr("Цена"));

    ui->tableView->setModel(queryModel);

    ui->tableView->horizontalHeader()->setDefaultSectionSize(maximumWidth());
    ui->tableView->resizeColumnsToContents();
}

void ListSparePart::closeWindow()
{
    close();
}

void ListSparePart::on_addSparePartButton_clicked()
{
    addSparePart = new AddSparePart;
    addSparePart->show();
    addSparePart->setAttribute(Qt::WA_DeleteOnClose);
}

void ListSparePart::showSparePartInfo(const QModelIndex &index)
{
    QString sparePartId = queryModel->data(queryModel->index(index.row(), 0)).toString();

    viewSparePart = new ViewSparePart;
    viewSparePart->setValues(sparePartId);
    viewSparePart->show();
    viewSparePart->setAttribute(Qt::WA_DeleteOnClose);
}
