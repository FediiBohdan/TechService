#include "AddTask.h"
#include "ui_AddTask.h"

AddTask::AddTask(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddTask)
{
    ui->setupUi(this);

    QDir tempDirDB = QDir::currentPath(); tempDirDB.cdUp(); QString dirDB = tempDirDB.path();

    listTasksDB = QSqlDatabase::addDatabase("QSQLITE");
    listTasksDB.setDatabaseName(dirDB + "\\CRM_AutoService\\ServiceStationDB.db");
    listTasksDB.open();

    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowFlags(windowFlags() & Qt::WindowMinimizeButtonHint);

    ui->contentErrorLabel->hide();
    ui->dateErrorLabel->hide();

    setDateAndTime();
}

AddTask::~AddTask()
{
    delete ui;
}

void AddTask::closeEvent(QCloseEvent*)
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

    QString isFulfilled = "0";

    queryTasks.prepare("INSERT INTO TasksTable (time, date, content, is_fulfilled) "
        "VALUES(?, ?, ?, ?)");

    queryTasks.addBindValue(time);
    queryTasks.addBindValue(date);
    queryTasks.addBindValue(content);
    queryTasks.addBindValue(isFulfilled);
    queryTasks.exec();

    QDialog::close();

    QMessageBox::information(this, tr("Уведомление"), tr("Задание успешно создано!"), QMessageBox::Ok);
}
