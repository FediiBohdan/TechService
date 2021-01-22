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

void ViewSparePart::closeEvent(QCloseEvent*)
{
    QDialog::close();

    listSparePart = new ListSparePart;
    listSparePart->show();
    listSparePart->setAttribute(Qt::WA_DeleteOnClose);
}

// Gets spare part's id from ListSpareParts
void ViewSparePart::setValues(const QString &id)
{
    sparePartId = id;

    QSqlQuery query(sparePartsDB);

    query.prepare("SELECT DISTINCT spare_part_name, manufacturer, quantity_in_stock, auto_compatibility, original, price "
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

void ViewSparePart::on_updateSparePartInfoButton_clicked()
{
    QDialog::hide();

    updateSparePart = new UpdateSparePart;
    updateSparePart->setValues(sparePartId);
    connect(updateSparePart, &UpdateSparePart::sendData, this, &ViewSparePart::receiveData);
    updateSparePart->show();
    updateSparePart->setAttribute(Qt::WA_DeleteOnClose);
}

void ViewSparePart::receiveData(bool update)
{
    if (update)
        QDialog::close();
    else
        QDialog::show();
}
