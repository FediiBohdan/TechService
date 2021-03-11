#ifndef LISTTASKS_H
#define LISTTASKS_H

#include "AddTask.h"
#include "ViewUpdateTask.h"

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
class ViewUpdateTask;

namespace Ui {
class ListTasks;
}

class ListTasks : public QDialog
{
    Q_OBJECT

public:
    explicit ListTasks(QWidget *parent = nullptr);
    ~ListTasks();

public slots:
    void closeWindow();

private:
    Ui::ListTasks *ui;

    QSqlDatabase listTasksTable = QSqlDatabase::database("TasksTable");

    QSqlDatabase listTasksDB;
    QPointer<QSqlQueryModel> queryModel;
    QPointer<QSqlQueryModel> queryModelCheckBox;
    QPointer<QSqlQueryModel> queryModelLabel;

    AddTask *addTask;
    ViewUpdateTask *viewUpdateTask;

    QWidget *addCheckBoxCompleted(int rowIndex);
    QWidget *addWidgetContent(int rowIndex);

private slots:
    void loadTable();
    void checkBoxStateChanged();
    void showTaskInfo(const QModelIndex &index);

    void on_addTaskButton_clicked();
    void on_updateButton_clicked();
};

#endif // LISTTASKS_H
