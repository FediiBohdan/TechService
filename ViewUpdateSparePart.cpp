#include "ViewUpdateSparePart.h"
#include "ui_ViewUpdateSparePart.h"

ViewUpdateSparePart::ViewUpdateSparePart(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ViewUpdateSparePart)
{
    ui->setupUi(this);

    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowFlags(windowFlags() & Qt::WindowMinimizeButtonHint);

    ui->errorLabel->hide();
}

ViewUpdateSparePart::~ViewUpdateSparePart()
{
    delete ui;
}

void ViewUpdateSparePart::closeEvent(QCloseEvent *)
{
    QDialog::close();

    listSpareParts = new ListSparePart;
    listSpareParts->show();
    listSpareParts->setAttribute(Qt::WA_DeleteOnClose);
}

void ViewUpdateSparePart::setValues(const QString &id)
{
    sparePartId = id;

    QSqlQuery query(sparePartsTable);

    query.prepare("SELECT DISTINCT spare_part_name, manufacturer, quantity_in_stock, auto_compatibility, original, price "
        "FROM spare_parts_catalogue WHERE id_spare_part = " + sparePartId);

    query.exec();
    query.next();

    ui->sparePartNameLine->setText(query.value(0).toString());
    ui->manufacturerLine->setText(query.value(1).toString());
    ui->quantityInStockLine->setText(query.value(2).toString());
    QString autoCompatibility = query.value(3).toString();
    autoCompatibility.replace("\n", ", ");
    ui->autoCompatibilityLine->setText(autoCompatibility);
    ui->isOriginalLine->setText(query.value(4).toString());
    ui->priceLine->setText(query.value(5).toString());
}

void ViewUpdateSparePart::on_saveUpdatedInfo_clicked()
{
    QSqlQuery querySpareParts(sparePartsTable);

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

    querySpareParts.prepare("UPDATE spare_parts_catalogue SET spare_part_name = ?, manufacturer = ?, quantity_in_stock = ?, auto_compatibility = ?, original = ?, price = ? "
        "WHERE id_spare_part = ?");

    querySpareParts.addBindValue(sparePartName);
    querySpareParts.addBindValue(manufacturer);
    querySpareParts.addBindValue(quantityInStock);
    querySpareParts.addBindValue(autoCompatibility);
    querySpareParts.addBindValue(isOriginal);
    querySpareParts.addBindValue(price);
    querySpareParts.addBindValue(sparePartId);
    querySpareParts.exec();

    QDialog::close();

    QMessageBox::information(this, tr("Уведомление"), tr("Информация о запчасти успешно обновлена!"), QMessageBox::Ok);
}

void ViewUpdateSparePart::on_updateSparePartInfoButton_clicked()
{
    ui->updateSparePartInfoButton->setEnabled(false);
    ui->sparePartNameLine->setReadOnly(false);
    ui->manufacturerLine->setReadOnly(false);
    ui->quantityInStockLine->setReadOnly(false);
    ui->autoCompatibilityLine->setReadOnly(false);
    ui->isOriginalLine->setReadOnly(false);
    ui->priceLine->setReadOnly(false);
    ui->deleteSparePartButton->setEnabled(true);
    ui->saveUpdatedInfo->setEnabled(true);
}

void ViewUpdateSparePart::on_deleteSparePartButton_clicked()
{
    QSqlQuery queryRemoveSparePart(sparePartsTable);

    int msgBox = QMessageBox::information(this, tr("Предупреждение"), tr("Вы уверены, что хотите удалить запчасть?"), QMessageBox::Ok, QMessageBox::Cancel);

    switch (msgBox)
    {
    case QMessageBox::Ok:

        queryRemoveSparePart.prepare("DELETE FROM spare_parts_catalogue WHERE id_spare_part = ?");
        queryRemoveSparePart.addBindValue(sparePartId);
        queryRemoveSparePart.exec();

        QDialog::close();

        QMessageBox::information(this, tr("Уведомление"), tr("Запчасть успешно удалена!"), QMessageBox::Ok);
        break;
    case QMessageBox::Cancel:
        return;
        break;
    default:
        break;
    }
}
