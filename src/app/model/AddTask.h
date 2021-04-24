#ifndef ADDTASK_H
#define ADDTASK_H

#include "Global.h"
#include "ListTasks.h"

#include <QDate>
#include <QTime>
#include <QDebug>
#include <QDialog>
#include <QPointer>
#include <QSqlQuery>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQueryModel>

class ListTasks;

namespace Ui {
class AddTask;
}

class AddTask : public QDialog
{
    Q_OBJECT

public:
    explicit AddTask(QWidget *parent = nullptr);
    ~AddTask();

private slots:
    void setDateAndTime();

    void on_createTaskButton_clicked();

    void closeEvent(QCloseEvent *);

private:
    Ui::AddTask *ui;

    QSqlDatabase listTasksTable;
    QPointer<QSqlQueryModel> queryModel;

    ListTasks *listTasksDialog;
};

#endif // ADDTASK_H
