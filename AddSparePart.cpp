#include "AddSparePart.h"
#include "ui_AddSparePart.h"

AddSparePart::AddSparePart(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddSparePart)
{
    ui->setupUi(this);

    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowFlags(windowFlags() & Qt::WindowMinimizeButtonHint);

    sparePartsDB = QSqlDatabase::addDatabase("QSQLITE");
    sparePartsDB.setDatabaseName("C:\\Users\\BohdanF\\Documents\\Diploma\\CRM_AutoService\\ServiceStationDB.db");
    sparePartsDB.open();
}

AddSparePart::~AddSparePart()
{
    delete ui;
}

void AddSparePart::on_createSparePartButton_clicked()
{
    // Insertion into order table
    QSqlQuery queryOrders(sparePartsDB);

    QString sparePartName = ui->sparePartNameLine->text();
    QString manufacturer = ui->manufacturerLine->text();
    QString quantityInStock = ui->quantityInStockLine->text();
    QString autoCompatibility = ui->autoCompatibilityLine->text();
    QString isOriginal = ui->isOriginalLine->text();
    QString price = ui->priceLine->text();

    queryOrders.prepare("INSERT INTO SparePartsCatalogue (spare_name, manufacturer, quantity_in_stock, auto_compatibility, original, price) "
        "VALUES(?, ?, ?, ?, ?, ?)");

    queryOrders.addBindValue(sparePartName);
    queryOrders.addBindValue(manufacturer);
    queryOrders.addBindValue(quantityInStock);
    queryOrders.addBindValue(autoCompatibility);
    queryOrders.addBindValue(isOriginal);
    queryOrders.addBindValue(price);
    queryOrders.exec();

    QDialog::close();

    QMessageBox::information(this, tr("Уведомление"), tr("Запчасть успешно добавлена в каталог!"), QMessageBox::Ok);
}
