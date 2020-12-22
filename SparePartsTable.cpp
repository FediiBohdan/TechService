#include "SparePartsTable.h"
#include "ui_SparePartsTable.h"

SparePartsTable::SparePartsTable(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SparePartsTable)
{
    ui->setupUi(this);
}

SparePartsTable::~SparePartsTable()
{
    delete ui;
}
