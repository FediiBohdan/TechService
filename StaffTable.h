#ifndef STAFFTABLE_H
#define STAFFTABLE_H

#include <QDialog>
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QPointer>

namespace Ui {
class StaffTable;
}

class StaffTable : public QDialog
{
    Q_OBJECT

private slots:
    void loadTable();

public:
    explicit StaffTable(QWidget *parent = nullptr);
    ~StaffTable();

private:
    Ui::StaffTable *ui;

    QSqlDatabase staffTable = QSqlDatabase::database("StaffTable");

    QSqlDatabase db;
    QPointer<QSqlQueryModel> queryModel;
};

#endif // STAFFTABLE_H
