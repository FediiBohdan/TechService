#ifndef LISTTASKS_H
#define LISTTASKS_H

#include "AddTask.h"
#include "ViewTask.h"

#include <QDialog>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QPointer>
#include <QSqlQuery>
#include <QDebug>

class AddTask;
class ViewTask;

namespace Ui {
class ListTasks;
}

class ListTasks : public QDialog
{
    Q_OBJECT

public:
    explicit ListTasks(QWidget *parent = nullptr);
    ~ListTasks();

private:
    Ui::ListTasks *ui;

    QSqlDatabase listTasks = QSqlDatabase::database("TasksTable");

    QSqlDatabase listTasksDB;
    QPointer<QSqlQueryModel> queryModel;

    AddTask *addTask;
    ViewTask *viewTask;

private slots:
    void loadTable();
    void showTaskInfo(const QModelIndex &index);

    void on_addTaskButton_clicked();

    void on_updateButton_clicked();

public slots:
    void closeWindow();
};

#endif // LISTTASKS_H
