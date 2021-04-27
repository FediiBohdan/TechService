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

/**
 * Event on window close.
 */
void ViewUpdateTask::closeEvent(QCloseEvent *)
{
    QDialog::close();

    listTasks = new ListTasks;
    listTasks->show();
    listTasks->setAttribute(Qt::WA_DeleteOnClose);
}

/**
 * Sets task information to corresponding fields.
 */
void ViewUpdateTask::setValues(const QString &id)
{
    taskId = id;

    QSqlQuery query(listTasksTable);

    query.prepare("SELECT DISTINCT time, date, content, is_fulfilled FROM tasks_table WHERE id_to_do_list = " + taskId);
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

/**
 * Checks input information and updates task in DB.
 */
void ViewUpdateTask::on_saveUpdatedInfo_clicked()
{
    QSqlQuery queryTasks(listTasksTable);

    QString time = ui->timeLine->text();
    QString date = ui->dateLine->text();
    QString content = ui->contentLine->toPlainText();

    if ((time.isEmpty()) || (date.isEmpty()) || (content.isEmpty()))
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

    queryTasks.prepare("UPDATE tasks_table SET time = ?, date = ?, content = ?, is_fulfilled = ? WHERE id_to_do_list = ?");

    queryTasks.addBindValue(time);
    queryTasks.addBindValue(date);
    queryTasks.addBindValue(content);
    queryTasks.addBindValue(isFulfilled);
    queryTasks.addBindValue(taskId);
    queryTasks.exec();

    QDialog::close();

    QMessageBox::information(this, tr("Уведомление"), tr("Задание успешно обновлено!"), QMessageBox::Ok);
}

/**
 * Allows information update.
 */
void ViewUpdateTask::on_updateTaskButton_clicked()
{
    ui->updateTaskButton->setEnabled(false);
    ui->checkBox->setCheckable(true);
    ui->contentLine->setReadOnly(false);
    ui->deleteTaskButton->setEnabled(true);
    ui->saveUpdatedInfo->setEnabled(true);
}

/**
 * Delets task from DB.
 */
void ViewUpdateTask::on_deleteTaskButton_clicked()
{
    QSqlQuery queryRemoveTask(listTasksTable);

    int msgBox = QMessageBox::information(this, tr("Предупреждение"), tr("Вы уверены, что хотите удалить задание?"), QMessageBox::Ok, QMessageBox::Cancel);

    switch (msgBox)
    {
    case QMessageBox::Ok:

        queryRemoveTask.prepare("DELETE FROM tasks_table WHERE id_to_do_list = ?");
        queryRemoveTask.addBindValue(taskId);
        queryRemoveTask.exec();

        QDialog::close();

        QMessageBox::information(this, tr("Уведомление"), tr("Задание успешно удалено!"), QMessageBox::Ok);
        break;
    case QMessageBox::Cancel:
        return;
        break;
    default:
        break;
    }
}
