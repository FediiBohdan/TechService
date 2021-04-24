#include "AddSparePart.h"
#include "ui_AddSparePart.h"

AddSparePart::AddSparePart(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddSparePart)
{
    ui->setupUi(this);

    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowFlags(windowFlags() & Qt::WindowMinimizeButtonHint);

    ui->errorLabel->hide();
}

AddSparePart::~AddSparePart()
{
    delete ui;
}

void AddSparePart::closeEvent(QCloseEvent *)
{
    QDialog::close();

    listSpareParts = new ListSparePart;
    listSpareParts->show();
    listSpareParts->setAttribute(Qt::WA_DeleteOnClose);
}

void AddSparePart::on_createSparePartButton_clicked()
{
    QSqlQuery queryOrders(sparePartsTable);

    QString sparePartName = ui->sparePartNameLine->text();
    QString manufacturer = ui->manufacturerLine->text();
    QString quantityInStock = ui->quantityInStockLine->text();
    QString autoCompatibility = ui->autoCompatibilityLine->text();
    QString isOriginal = ui->isOriginalLine->text();
    QString price = ui->priceLine->text();

    if ((sparePartName.isEmpty()) || (manufacturer.isEmpty()) || (quantityInStock.isEmpty()) || (autoCompatibility.isEmpty()) ||
            (isOriginal.isEmpty()) || (price.isEmpty()))
    {
        ui->errorLabel->show();
        return;
    }

    autoCompatibility.replace(", ", "\n");

    queryOrders.prepare("INSERT INTO spare_parts_catalogue (spare_part_name, manufacturer, quantity_in_stock, auto_compatibility, original, price) "
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
