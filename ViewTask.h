#ifndef VIEWTASK_H
#define VIEWTASK_H

#include "ListTasks.h"
#include "UpdateTask.h"

#include <QDialog>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QPointer>
#include <QSqlQuery>
#include <QDebug>

class ListTasks;
class UpdateTask;

namespace Ui {
class ViewTask;
}

class ViewTask : public QDialog
{
    Q_OBJECT

public:
    explicit ViewTask(QWidget *parent = nullptr);
    ~ViewTask();

public slots:
    void setValues(const QString &id);
    void receiveData(bool update);

private:
    Ui::ViewTask *ui;

    QString taskId;

    QSqlDatabase listTasksTable = QSqlDatabase::database("TasksTable");

    QSqlDatabase listTasksDB;
    QPointer<QSqlQueryModel> queryModel;

    ListTasks *listTasks;
    UpdateTask *updateTask;

private slots:
    void closeEvent(QCloseEvent*);
    void on_updateTaskButton_clicked();
};

#endif // VIEWTASK_H
