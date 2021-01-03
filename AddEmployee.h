#ifndef ADDEMPLOYEE_H
#define ADDEMPLOYEE_H

#include <QDialog>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QPointer>
#include <QSqlQuery>
#include <QDebug>

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

private:
    Ui::AddEmployee *ui;

    QSqlDatabase listEmployees = QSqlDatabase::database("ListEmployees");
    QSqlDatabase employeeDB;
    QPointer<QSqlQueryModel> queryModel;
};

#endif // ADDEMPLOYEE_H
