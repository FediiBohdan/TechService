#include "UpdateSparePart.h"
#include "ui_UpdateSparePart.h"

UpdateSparePart::UpdateSparePart(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UpdateSparePart)
{
    ui->setupUi(this);

    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowFlags(windowFlags() & Qt::WindowMinimizeButtonHint);

    ui->errorLabel->hide();
}

UpdateSparePart::~UpdateSparePart()
{
    delete ui;
}

void UpdateSparePart::setValues(const QString& id)
{
    sparePartId = id;

    QSqlQuery query(sparePartsDB);

    query.prepare("SELECT DISTINCT spare_name, manufacturer, quantity_in_stock, auto_compatibility, original, price "
        "FROM SparePartsCatalogue WHERE id_spare_part = " + sparePartId);

    query.exec();
    query.next();

    ui->sparePartNameLine->setText(query.value(0).toString());
    ui->manufacturerLine->setText(query.value(1).toString());
    ui->quantityInStockLine->setText(query.value(2).toString());
    ui->autoCompatibilityLine->setText(query.value(3).toString());
    ui->isOriginalLine->setText(query.value(4).toString());
    ui->priceLine->setText(query.value(5).toString());
}

void UpdateSparePart::on_backToViewInfoButton_clicked()
{
    emit sendData(false);

    QDialog::close();
}

void UpdateSparePart::on_saveUpdatedInfo_clicked()
{
    QSqlQuery queryOrders(sparePartsDB);

    QString sparePartName = ui->sparePartNameLine->text();
    QString manufacturer = ui->manufacturerLine->text();
    QString quantityInStock = ui->quantityInStockLine->text();
    QString autoCompatibility = ui->autoCompatibilityLine->text();
    QString isOriginal = ui->isOriginalLine->text();
    QString price = ui->priceLine->text();

    if (sparePartName.isEmpty() || manufacturer.isEmpty() || quantityInStock.isEmpty() || autoCompatibility.isEmpty() ||
            isOriginal.isEmpty() || price.isEmpty())
    {
        ui->errorLabel->show();
        return;
    }

    queryOrders.prepare("UPDATE SparePartsCatalogue SET spare_name = ?, manufacturer = ?, quantity_in_stock = ?, auto_compatibility = ?, original = ?, price = ? "
        "WHERE id_spare_part = ?");

    queryOrders.addBindValue(sparePartName);
    queryOrders.addBindValue(manufacturer);
    queryOrders.addBindValue(quantityInStock);
    queryOrders.addBindValue(autoCompatibility);
    queryOrders.addBindValue(isOriginal);
    queryOrders.addBindValue(price);
    queryOrders.addBindValue(sparePartId);
    queryOrders.exec();

    QDialog::close();

    QMessageBox::information(this, tr("Уведомление"), tr("Информация о запчасти успешно обновлена!"), QMessageBox::Ok);
}
