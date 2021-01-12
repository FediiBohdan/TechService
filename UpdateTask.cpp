#include "UpdateTask.h"
#include "ui_UpdateTask.h"

UpdateTask::UpdateTask(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UpdateTask)
{
    ui->setupUi(this);
}

UpdateTask::~UpdateTask()
{
    delete ui;
}

void UpdateTask::setValues(const QString &id)
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

void UpdateTask::on_backToViewInfoButton_clicked()
{
    emit sendData(false);

    QDialog::close();
}

void UpdateTask::on_saveUpdatedInfo_clicked()
{
    QSqlQuery queryTasks(listTasksDB);

    QString time = ui->timeLine->text();
    QString date = ui->dateLine->text();
    QString content = ui->contentLine->text();
    QString isFulfilled = ui->isFulfilledLine->text();

    queryTasks.prepare("UPDATE TasksTable SET time = ?, date = ?, content = ?, is_fulfilled = ? WHERE id_to_do_list = ?");

    queryTasks.addBindValue(time);
    queryTasks.addBindValue(date);
    queryTasks.addBindValue(content);
    queryTasks.addBindValue(isFulfilled);
    queryTasks.addBindValue(taskId);
    queryTasks.exec();

    QDialog::close();

    QMessageBox::information(this, tr("Уведомление"), tr("Задание успешно обновлено!"), QMessageBox::Ok);
}
