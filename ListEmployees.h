#ifndef LISTEMPLOYEES_H
#define LISTEMPLOYEES_H

#include "AddEmployee.h"
#include "ViewUpdateEmployee.h"

#include <QFile>
#include <QLabel>
#include <QDebug>
#include <QDialog>
#include <QPointer>
#include <QTextStream>
#include <QFileDialog>
#include <QMessageBox>
#include <QSqlQueryModel>

class ViewUpdateEmployee;
class AddEmployee;

namespace Ui {
class ListEmployees;
}

class ListEmployees : public QDialog
{
    Q_OBJECT

public:
    explicit ListEmployees(QWidget *parent = nullptr);
    ~ListEmployees();

private slots:
    void loadTable();
    void saveAsCSV(QString fileName);
    void showEmployeeInfo(const QModelIndex &index);

    void on_addWorkerButton_clicked();
    void on_updateButton_clicked();
    void on_csvExportButton_clicked();

private:
    Ui::ListEmployees *ui;

    QPointer<QSqlQueryModel> queryModel;

    AddEmployee *addEmployee;
    ViewUpdateEmployee *viewUpdateEmployees;
};

#endif // LISTEMPLOYEES_H
