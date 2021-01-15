#include "ListTasks.h"
#include "ui_ListTasks.h"

ListTasks::ListTasks(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ListTasks)
{
    ui->setupUi(this);

    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowFlags(windowFlags() & Qt::WindowMinimizeButtonHint);

    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);

    QDialog::showNormal();
    QDialog::showMaximized();

    QDir tempDirDB = QDir::currentPath(); tempDirDB.cdUp(); QString dirDB = tempDirDB.path();

    listTasksDB = QSqlDatabase::addDatabase("QSQLITE");
    listTasksDB.setDatabaseName(dirDB + "\\CRM_AutoService\\ServiceStationDB.db");
    listTasksDB.open();

    connect(ui->tableView, &QAbstractItemView::doubleClicked, this, &ListTasks::showTaskInfo);

    loadTable();
}

ListTasks::~ListTasks()
{
    delete ui;
}

void ListTasks::loadTable()
{
    queryModel = new QSqlQueryModel(this);

    QString queryString;

    queryString = "SELECT id_to_do_list, time, date, content, is_fulfilled "
        "FROM TasksTable";

    queryModel->setQuery(queryString, listTasks);

    queryModel->setHeaderData(0, Qt::Horizontal, tr("id"));
    queryModel->setHeaderData(1, Qt::Horizontal, tr("Время"));
    queryModel->setHeaderData(2, Qt::Horizontal, tr("Дата"));
    queryModel->setHeaderData(3, Qt::Horizontal, tr("Содержание"));
    queryModel->setHeaderData(4, Qt::Horizontal, tr("Выполнено"));

    ui->tableView->setModel(queryModel);

    ui->tableView->setColumnHidden(0, true);

    ui->tableView->horizontalHeader()->setDefaultSectionSize(maximumWidth());
    ui->tableView->resizeColumnsToContents();
}

void ListTasks::closeWindow()
{
    close();
}

void ListTasks::on_addTaskButton_clicked()
{
    addTask = new AddTask;
    addTask->show();
    addTask->setAttribute(Qt::WA_DeleteOnClose);
}

void ListTasks::showTaskInfo(const QModelIndex &index)
{
    QDialog::hide();

    QString taskId = queryModel->data(queryModel->index(index.row(), 0)).toString();

    viewTask = new ViewTask;
    viewTask->setValues(taskId);
    viewTask->show();
    viewTask->setAttribute(Qt::WA_DeleteOnClose);
}

void ListTasks::on_updateButton_clicked()
{
   queryModel->setQuery(NULL);

   loadTable();
}
