#ifndef ADDTASK_H
#define ADDTASK_H

#include "ListTasks.h"

#include <QDialog>
#include <QDebug>
#include <QDate>
#include <QTime>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QPointer>
#include <QSqlQuery>
#include <QDir>

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

    void closeEvent(QCloseEvent*);

private:
    Ui::AddTask *ui;

    QSqlDatabase listTasks = QSqlDatabase::database("TasksTable");
    QSqlDatabase listTasksDB;
    QPointer<QSqlQueryModel> queryModel;

    ListTasks *listTasksDialog;
};

#endif // ADDTASK_H
