#ifndef VIEWUPDATETASK_H
#define VIEWUPDATETASK_H

#include "ListTasks.h"

#include <QDialog>
#include <QSqlDatabase>

class ListTasks;

namespace Ui {
class ViewUpdateTask;
}

class ViewUpdateTask : public QDialog
{
    Q_OBJECT

signals:
    void sendData(bool update);

public:
    explicit ViewUpdateTask(QWidget *parent = nullptr);
    ~ViewUpdateTask();

public slots:
    void setValues(const QString &id);

private slots:
    void on_saveUpdatedInfo_clicked();
    void on_updateTaskButton_clicked();

    void closeEvent(QCloseEvent *);

private:
    Ui::ViewUpdateTask *ui;

    QSqlDatabase listTasksTable;
    QString taskId;

    ListTasks *listTasks;
};

#endif // VIEWUPDATETASK_H
