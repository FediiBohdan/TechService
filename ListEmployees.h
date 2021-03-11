#ifndef LISTEMPLOYEES_H
#define LISTEMPLOYEES_H

#include "AddEmployee.h"
#include "ViewUpdateEmployee.h"

#include <QDialog>
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QPointer>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QDebug>
#include <QLabel>
#include <QFile>
#include <QTextStream>
#include <QFileDialog>

class ViewUpdateEmployee;
class AddEmployee;

namespace Ui {
class ListEmployees;
}

class ListEmployees : public QDialog
{
    Q_OBJECT

private slots:
    void loadTable();
    void saveAsCSV(QString filename);
    void showEmployeeInfo(const QModelIndex &index);

    void on_addWorkerButton_clicked();
    void on_updateButton_clicked();
    void on_csvExportButton_clicked();

public:
    explicit ListEmployees(QWidget *parent = nullptr);
    ~ListEmployees();

private:
    Ui::ListEmployees *ui;

    QSqlDatabase employeesTable = QSqlDatabase::database("EmployeesTable");
    QSqlDatabase employeesDB;
    QPointer<QSqlQueryModel> queryModel;
    QPointer<QSqlQueryModel> queryModelLabel;

    AddEmployee *addEmployee;
    ViewUpdateEmployee *viewUpdateEmployees;
};

#endif // LISTEMPLOYEES_H
