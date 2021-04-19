#include "AddTask.h"
#include "ui_AddTask.h"

AddTask::AddTask(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddTask)
{
    ui->setupUi(this);

    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowFlags(windowFlags() & Qt::WindowMinimizeButtonHint);

    ui->errorLabel->setStyleSheet("color: transparent");

    setDateAndTime();
}

AddTask::~AddTask()
{
    delete ui;
}

void AddTask::closeEvent(QCloseEvent *)
{
    QDialog::close();

    listTasksDialog = new ListTasks;
    listTasksDialog->show();
    listTasksDialog->setAttribute(Qt::WA_DeleteOnClose);
}

void AddTask::setDateAndTime()
{
    QDate currentDate = QDate::currentDate();
    QTime currentTime = QTime::currentTime();
    ui->dateLine->setText(currentDate.toString(Qt::SystemLocaleDate));
    ui->timeLine->setText(currentTime.toString(Qt::SystemLocaleDate));
}

void AddTask::on_createTaskButton_clicked()
{
    QSqlQuery queryTasks(listTasksTable);

    QString userLogin = global::getSettingsValue("userLogin", "settings").toString();

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

    QString isFulfilled = "0";

    if (!userLogin.isEmpty())
    {
        queryTasks.prepare("INSERT INTO tasks_table (time, date, content, is_fulfilled, user) "
            "VALUES(?, ?, ?, ?, ?)");

        queryTasks.addBindValue(time);
        queryTasks.addBindValue(date);
        queryTasks.addBindValue(content);
        queryTasks.addBindValue(isFulfilled);
        queryTasks.addBindValue(userLogin);
        queryTasks.exec();

        QDialog::close();

        QMessageBox::information(this, tr("Уведомление"), tr("Задание успешно создано!"), QMessageBox::Ok);
    }
    else
        QMessageBox::information(this, tr("Предупреждение"), tr("Пользователь не зарегистрирован!"), QMessageBox::Ok);
}
