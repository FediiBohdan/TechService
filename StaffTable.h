#ifndef STAFFTABLE_H
#define STAFFTABLE_H

#include <AddWorker.h>

#include <QDialog>
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QPointer>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QDebug>

namespace Ui {
class StaffTable;
}

class StaffTable : public QDialog
{
    Q_OBJECT

private slots:
    void loadTable();

    void on_addWorkerButton_clicked();

public:
    explicit StaffTable(QWidget *parent = nullptr);
    ~StaffTable();

private:
    Ui::StaffTable *ui;

    QSqlDatabase staffTable = QSqlDatabase::database("StaffTable");

    QSqlDatabase db;
    QPointer<QSqlQueryModel> queryModel;

    AddWorker *addWorker;
};

#endif // STAFFTABLE_H
