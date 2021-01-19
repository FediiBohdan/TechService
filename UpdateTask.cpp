#include "UpdateTask.h"
#include "ui_UpdateTask.h"

UpdateTask::UpdateTask(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UpdateTask)
{
    ui->setupUi(this);

    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowFlags(windowFlags() & Qt::WindowMinimizeButtonHint);

    ui->dateErrorLabel->hide();
    ui->contentErrorLabel->hide();
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

    QString isFulfilled = query.value(3).toString();

    if (isFulfilled == "1")
        ui->checkBox->setChecked(true);
    else
        ui->checkBox->setChecked(false);
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

    bool error = false;

    if (time.isEmpty() || date.isEmpty())
    {
        ui->dateErrorLabel->show();
        error = true;
    }
    else
        ui->dateErrorLabel->hide();

    QString content = ui->contentLine->toPlainText();

    if (content.isEmpty())
    {
        ui->contentErrorLabel->show();
        error = true;
    }
    else
        ui->contentErrorLabel->hide();

    if (error)
        return;

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
