#include "ViewUpdateTask.h"
#include "ui_ViewUpdateTask.h"

ViewUpdateTask::ViewUpdateTask(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ViewUpdateTask)
{
    ui->setupUi(this);

    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowFlags(windowFlags() & Qt::WindowMinimizeButtonHint);

    ui->errorLabel->setStyleSheet("color: transparent");
}

ViewUpdateTask::~ViewUpdateTask()
{
    delete ui;
}

void ViewUpdateTask::setValues(const QString &id)
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

    QString isFulfilled = query.value(3).toString();

    if (isFulfilled == "1")
        ui->checkBox->setChecked(true);
    else
        ui->checkBox->setChecked(false);
}

void ViewUpdateTask::on_saveUpdatedInfo_clicked()
{
    QSqlQuery queryTasks(listTasksDB);

    QString time = ui->timeLine->text();
    QString date = ui->dateLine->text();
    QString content = ui->contentLine->toPlainText();

    if (time.isEmpty() || date.isEmpty() || content.isEmpty())
    {
        ui->errorLabel->setStyleSheet("color: red");
        return;
    }
    else
        ui->errorLabel->setStyleSheet("color: transparent");

    QString isFulfilled;

    if (ui->checkBox->isChecked() == true)
        isFulfilled = "1";
    else
        isFulfilled = "0";

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

void ViewUpdateTask::on_updateTaskButton_clicked()
{
    ui->updateTaskButton->setEnabled(false);
    ui->checkBox->setCheckable(true);
    ui->contentLine->setReadOnly(false);
    ui->saveUpdatedInfo->setEnabled(true);
}
