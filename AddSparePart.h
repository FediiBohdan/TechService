#ifndef ADDSPAREPART_H
#define ADDSPAREPART_H

#include <ListSpareParts.h>

#include <QDialog>
#include <QDebug>
#include <QDate>
#include <QTime>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QPointer>
#include <QSqlQuery>
#include <QDir>

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

    void closeEvent(QCloseEvent*);

private:
    Ui::AddSparePart *ui;

    QSqlDatabase sparePart = QSqlDatabase::database("SparePartsCatalogue");
    QSqlDatabase sparePartsDB;
    QPointer<QSqlQueryModel> queryModel;

    ListSparePart *listSpareParts;
};

#endif // ADDSPAREPART_H
