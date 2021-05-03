#ifndef ADDEMPLOYEE_H
#define ADDEMPLOYEE_H

#include "ListEmployees.h"

#include <QDebug>
#include <QDialog>
#include <QPointer>
#include <QSqlQuery>
#include <QMessageBox>
#include <QSqlDatabase>

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

    QSqlDatabase employeeTable;

    ListEmployees *listEmployees;
};

#endif // ADDEMPLOYEE_H
