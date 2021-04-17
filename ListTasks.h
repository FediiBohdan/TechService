#ifndef LISTTASKS_H
#define LISTTASKS_H

#include "AddTask.h"
#include "ViewUpdateTask.h"

#include <QFile>
#include <QLabel>
#include <QDebug>
#include <QDialog>
#include <QPointer>
#include <QCheckBox>
#include <QTextStream>
#include <QFileDialog>
#include <QMessageBox>
#include <QSqlQueryModel>

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

    QSqlDatabase listTasksTable;
    QPointer<QSqlQueryModel> queryModel;    
    QPointer<QSqlQueryModel> queryModelLabel;
    QPointer<QSqlQueryModel> queryModelCheckBox;

    AddTask *addTask;
    ViewUpdateTask *viewUpdateTask;

    QWidget *addWidgetContent(int rowIndex);
    QWidget *addCheckBoxCompleted(int rowIndex);

private slots:
    void loadTable();
    void checkBoxStateChanged();
    void showTaskInfo(const QModelIndex &index);

    void on_addTaskButton_clicked();
    void on_updateButton_clicked();
};

#endif // LISTTASKS_H
