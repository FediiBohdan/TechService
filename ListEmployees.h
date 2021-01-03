#ifndef LISTEMPLOYEES_H
#define LISTEMPLOYEES_H

#include <AddEmployee.h>

#include <QDialog>
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QPointer>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QDebug>

namespace Ui {
class ListEmployees;
}

class ListEmployees : public QDialog
{
    Q_OBJECT

private slots:
    void loadTable();

    void on_addWorkerButton_clicked();

public:
    explicit ListEmployees(QWidget *parent = nullptr);
    ~ListEmployees();

private:
    Ui::ListEmployees *ui;

    QSqlDatabase employeesTable = QSqlDatabase::database("EmployeesTable");
    QSqlDatabase employeesDB;
    QPointer<QSqlQueryModel> queryModel;

    AddEmployee *addEmployee;
};

#endif // LISTEMPLOYEES_H
