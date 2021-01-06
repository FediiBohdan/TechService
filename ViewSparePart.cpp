#include "ViewSparePart.h"
#include "ui_ViewSparePart.h"

ViewSparePart::ViewSparePart(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ViewSparePart)
{
    ui->setupUi(this);

    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowFlags(windowFlags() & Qt::WindowMinimizeButtonHint);
}

ViewSparePart::~ViewSparePart()
{
    delete ui;
}

// Gets spare part's id from ListSpareParts
void ViewSparePart::setValues(const QString &id)
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
