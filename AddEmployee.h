#ifndef ADDEMPLOYEE_H
#define ADDEMPLOYEE_H

#include "ListEmployees.h"

#include <QDialog>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QPointer>
#include <QSqlQuery>
#include <QDebug>
#include <QDir>

class ListEmployees;

namespace Ui {
class AddEmployee;
}

class AddEmployee : public QDialog
{
    Q_OBJECT

public:
    explicit AddEmployee(QWidget *parent = nullptr);
    ~AddEmployee();

private slots:
    void on_saveWorkerButton_clicked();

    void closeEvent(QCloseEvent *);

private:
    Ui::AddEmployee *ui;

    QSqlDatabase listEmployeesTable = QSqlDatabase::database("EmployeesTable");
    QSqlDatabase employeeDB;
    QPointer<QSqlQueryModel> queryModel;

    ListEmployees *listEmployees;
};

#endif // ADDEMPLOYEE_H
