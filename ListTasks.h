#ifndef LISTTASKS_H
#define LISTTASKS_H

#include "AddTask.h"
#include "ViewTask.h"
#include "UpdateTask.h"

#include <QDialog>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QPointer>
#include <QSqlQuery>
#include <QDebug>
#include <QCheckBox>
#include <QLabel>

class AddTask;
class ViewTask;
class UpdateTask;

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

    QSqlDatabase listTasksTable = QSqlDatabase::database("TasksTable");

    QSqlDatabase listTasksDB;
    QPointer<QSqlQueryModel> queryModel;
    QPointer<QSqlQueryModel> queryModelCheckBox;
    QPointer<QSqlQueryModel> queryModelLabel;

    AddTask *addTask;
    ViewTask *viewTask;

    QWidget *addCheckBoxCompleted(int row_index);
    QWidget *addWidgetContent(int row_index);

private slots:
    void loadTable();
    void checkBoxStateChanged();
    void showTaskInfo(const QModelIndex &index);

    void on_addTaskButton_clicked();
    void on_updateButton_clicked();

public slots:
    void closeWindow();
};

#endif // LISTTASKS_H
