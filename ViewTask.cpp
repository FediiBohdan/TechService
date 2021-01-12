#include "ViewTask.h"
#include "ui_ViewTask.h"

ViewTask::ViewTask(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ViewTask)
{
    ui->setupUi(this);
}

ViewTask::~ViewTask()
{
    delete ui;
}

void ViewTask::closeEvent(QCloseEvent*)
{
    QDialog::hide();

    listTasks = new ListTasks;
    listTasks->show();
    listTasks->setAttribute(Qt::WA_DeleteOnClose);
}

// Gets task's id from ListTasks
void ViewTask::setValues(const QString &id)
{
    taskId = id;

    QSqlQuery query(listTasksDB);

    query.prepare("SELECT DISTINCT time, date, content, is_fulfilled "
        "FROM TasksTable WHERE id_to_do_list = " + taskId);

    query.exec();
    query.next();

    ui->timeLine->setText(query.value(0).toString());
    ui->dateLine->setText(query.value(1).toString());
    ui->contentLine->setText(query.value(2).toString());
    ui->isFulfilledLine->setText(query.value(3).toString());
}

void ViewTask::receiveData(bool update)
{
    if (update)
        QDialog::close();
    else
        QDialog::show();
}

void ViewTask::on_updateTaskButton_clicked()
{
    QDialog::hide();

    updateTask = new UpdateTask;
    updateTask->setValues(taskId);
    connect(updateTask, &UpdateTask::sendData, this, &ViewTask::receiveData);
    updateTask->show();
    updateTask->setAttribute(Qt::WA_DeleteOnClose);
}
