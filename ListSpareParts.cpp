#include "ListSpareParts.h"
#include "ui_ListSpareParts.h"

ListSparePart::ListSparePart(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ListSparePart)
{
    ui->setupUi(this);

    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowFlags(windowFlags() & Qt::WindowMinimizeButtonHint);

    QDialog::showMaximized();

    //connect(startWindow, &StartWindow::closeAllWindowsExceptCurrent, this, &ListSparePart::closeWindow);

    QDir tempDirDB = QDir::currentPath(); tempDirDB.cdUp(); QString dirDB = tempDirDB.path();

    listSparePartsDB = QSqlDatabase::addDatabase("QSQLITE");
    listSparePartsDB.setDatabaseName(dirDB + "\\CRM_AutoService\\ServiceStationDB.db");
    listSparePartsDB.open();

    connect(ui->tableView, &QAbstractItemView::doubleClicked, this, &ListSparePart::showSparePartInfo);

    searchFlag = false;
    autoSearchFlag = false;

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
    queryString = "SELECT id_spare_part, spare_part_name, manufacturer, quantity_in_stock, auto_compatibility, original, "
        "price FROM SparePartsCatalogue ";

    QString searchString;

    if (searchFlag)
        searchString.append("WHERE spare_part_name LIKE '%" + ui->sparePartSearch->text() + "%' GROUP BY id_spare_part ORDER BY spare_part_name ASC");

    else if (autoSearchFlag)
        searchString.append("WHERE auto_compatibility LIKE '%" + autoModel + "%' GROUP BY id_spare_part ORDER BY spare_part_name ASC");

    queryString.append(searchString);

    queryModel->setQuery(queryString);

    queryModel->setHeaderData(0, Qt::Horizontal, tr("id"));
    queryModel->setHeaderData(1, Qt::Horizontal, tr("Название"));
    queryModel->setHeaderData(2, Qt::Horizontal, tr("Производитель"));
    queryModel->setHeaderData(3, Qt::Horizontal, tr("Количество в наличии"));
    queryModel->setHeaderData(4, Qt::Horizontal, tr("Совместимость с моделями"));
    queryModel->setHeaderData(5, Qt::Horizontal, tr("Оригинал"));
    queryModel->setHeaderData(6, Qt::Horizontal, tr("Цена"));

    ui->tableView->setModel(queryModel);

    ui->tableView->setColumnHidden(0, true);

    ui->tableView->resizeColumnsToContents();
    ui->tableView->verticalHeader()->setSectionsClickable(false);
    ui->tableView->horizontalHeader()->setSectionsClickable(false);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->horizontalHeader()->setDefaultSectionSize(maximumWidth());
}

void ListSparePart::closeWindow()
{
    close();
}

void ListSparePart::on_addSparePartButton_clicked()
{
    QDialog::close();

    addSparePart = new AddSparePart;
    addSparePart->show();
    addSparePart->setAttribute(Qt::WA_DeleteOnClose);
}

void ListSparePart::showSparePartInfo(const QModelIndex &index)
{
    QDialog::close();

    QString sparePartId = queryModel->data(queryModel->index(index.row(), 0)).toString();

    viewSparePart = new ViewSparePart;
    viewSparePart->setValues(sparePartId);
    viewSparePart->show();
    viewSparePart->setAttribute(Qt::WA_DeleteOnClose);
}

void ListSparePart::on_sparePartSearch_returnPressed()
{    
    if (ui->sparePartSearch->text().isEmpty())
        searchFlag = false;
    else
        searchFlag = true;

    autoSearchFlag = false;

    on_updateButton_clicked();
}

void ListSparePart::on_toyotaSearchButton_clicked()
{
    autoSearchFlag = true;
    searchFlag = false;

    autoModel = "Toyota";

    on_updateButton_clicked();
}

void ListSparePart::on_mitsubishiSearchButton_clicked()
{
    autoSearchFlag = true;
    searchFlag = false;

    autoModel = "Mitsubishi";

    on_updateButton_clicked();
}

void ListSparePart::on_nissanSearchButton_clicked()
{
    autoSearchFlag = true;
    searchFlag = false;

    autoModel = "Nissan";

    on_updateButton_clicked();
}

void ListSparePart::on_mazdaSearchButton_clicked()
{
    autoSearchFlag = true;
    searchFlag = false;

    autoModel = "Mazda";

    on_updateButton_clicked();
}

void ListSparePart::on_hondaSearchButton_clicked()
{
    autoSearchFlag = true;
    searchFlag = false;

    autoModel = "Honda";

    on_updateButton_clicked();
}

void ListSparePart::on_lexusSearchButton_clicked()
{
    autoSearchFlag = true;
    searchFlag = false;

    autoModel = "Lexus";

    on_updateButton_clicked();
}

void ListSparePart::on_subaruSearchButton_clicked()
{
    autoSearchFlag = true;
    searchFlag = false;

    autoModel = "Subaru";

    on_updateButton_clicked();
}

void ListSparePart::on_suzukiSearchButton_clicked()
{
    autoSearchFlag = true;
    searchFlag = false;

    autoModel = "Suzuki";

    on_updateButton_clicked();
}

void ListSparePart::on_infinitiSearchButton_clicked()
{
    autoSearchFlag = true;
    searchFlag = false;

    autoModel = "Infiniti";

    on_updateButton_clicked();
}

void ListSparePart::on_isuzuSearchButton_clicked()
{
    autoSearchFlag = true;
    searchFlag = false;

    autoModel = "Isuzu";

    on_updateButton_clicked();
}

void ListSparePart::on_acuraSearchButton_clicked()
{
    autoSearchFlag = true;
    searchFlag = false;

    autoModel = "Acura";

    on_updateButton_clicked();
}

void ListSparePart::on_updateButton_clicked()
{
    queryModel->setQuery(NULL);

    loadTable();
}
