#ifndef ADDSPAREPART_H
#define ADDSPAREPART_H

#include "ListSpareParts.h"

#include <QDebug>
#include <QDialog>
#include <QPointer>
#include <QSqlQuery>
#include <QMessageBox>
#include <QSqlDatabase>

class ListSparePart;

namespace Ui {
class AddSparePart;
}

class AddSparePart : public QDialog
{
    Q_OBJECT

public:
    explicit AddSparePart(QWidget *parent = nullptr);
    ~AddSparePart();

private slots:
    void on_createSparePartButton_clicked();

    void closeEvent(QCloseEvent *);

private:
    Ui::AddSparePart *ui;

    QSqlDatabase sparePartsTable;

    ListSparePart *listSpareParts;
};

#endif // ADDSPAREPART_H
