#include "AnalyticsDialog.h"
#include "ui_AnalyticsDialog.h"

AnalyticsDialog::AnalyticsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AnalyticsDialog)
{
    ui->setupUi(this);
}

AnalyticsDialog::~AnalyticsDialog()
{
    delete ui;
}
