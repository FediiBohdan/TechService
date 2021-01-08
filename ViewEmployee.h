#ifndef VIEWEMPLOYEE_H
#define VIEWEMPLOYEE_H

#include "ListEmployees.h"

#include <QDialog>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QPointer>
#include <QSqlQuery>
#include <QDebug>

class ListEmployees;

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

private:
    Ui::ViewEmployee *ui;

    ListEmployees *listEmployees;

    QSqlDatabase employeesDB;

    QString employeeId;

private slots:
    void closeEvent(QCloseEvent*);
};

#endif // VIEWEMPLOYEE_H
