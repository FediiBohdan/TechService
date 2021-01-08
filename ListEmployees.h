#ifndef LISTEMPLOYEES_H
#define LISTEMPLOYEES_H

#include "AddEmployee.h"
#include "ViewEmployee.h"

#include <QDialog>
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QPointer>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QDebug>

class ViewEmployee;

namespace Ui {
class ListEmployees;
}

class ListEmployees : public QDialog
{
    Q_OBJECT

private slots:
    void loadTable();
    void showEmployeeInfo(const QModelIndex &index);

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
    ViewEmployee *viewEmployees;
};

#endif // LISTEMPLOYEES_H
