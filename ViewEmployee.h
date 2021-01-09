#ifndef VIEWEMPLOYEE_H
#define VIEWEMPLOYEE_H

#include "ListEmployees.h"
#include "UpdateEmployee.h"

#include <QDialog>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QPointer>
#include <QSqlQuery>
#include <QDebug>

class ListEmployees;
class UpdateEmployee;

namespace Ui {
class ViewEmployee;
}

class ViewEmployee : public QDialog
{
    Q_OBJECT

public:
    explicit ViewEmployee(QWidget *parent = nullptr);
    ~ViewEmployee();

public slots:
    void setValues(const QString &id);
    void receiveData(bool update);

private:
    Ui::ViewEmployee *ui;

    ListEmployees *listEmployees;
    UpdateEmployee *updateEmployee;

    QSqlDatabase employeesDB;

    QString employeeId;

private slots:
    void closeEvent(QCloseEvent*);
    void on_updateEmployeeInfoButton_clicked();
};

#endif // VIEWEMPLOYEE_H
