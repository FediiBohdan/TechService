#ifndef SPAREPARTSTABLE_H
#define SPAREPARTSTABLE_H

#include <QDialog>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QPointer>
#include <QSqlQuery>
#include <QDebug>

namespace Ui {
class SparePartsTable;
}

class SparePartsTable : public QDialog
{
    Q_OBJECT

public:
    explicit SparePartsTable(QWidget *parent = nullptr);
    ~SparePartsTable();

private:
    Ui::SparePartsTable *ui;

    QSqlDatabase sparePartsTable = QSqlDatabase::database("SparePartsCatalogue");

    QSqlDatabase db;
    QPointer<QSqlQueryModel> queryModel;

private slots:
    void loadTable();
};

#endif // SPAREPARTSTABLE_H
