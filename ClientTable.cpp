#include "ClientTable.h"
#include "ui_ClientTable.h"

ClientTable::ClientTable(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ClientTable)
{
    ui->setupUi(this);
}

ClientTable::~ClientTable()
{
    delete ui;
}
