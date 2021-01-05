#ifndef VIEWEMPLOYEE_H
#define VIEWEMPLOYEE_H

#include <QDialog>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QPointer>
#include <QSqlQuery>
#include <QDebug>

namespace Ui {
class ViewEmployee;
}

class ViewEmployee : public QDialog
{
    Q_OBJECT

public:
    explicit ViewEmployee(QWidget *parent = nullptr);
    ~ViewEmployee();

    void setValues(const QString &id);

private:
    Ui::ViewEmployee *ui;

    QSqlDatabase employeesTable = QSqlDatabase::database("EmployeesTable");
    QSqlDatabase employeesDB;

    QString employeeId;
};

#endif // VIEWEMPLOYEE_H
