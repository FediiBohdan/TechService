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

    connect(ui->tableView, &QAbstractItemView::doubleClicked, this, &ListTasks::showTaskInfo);

    loadTable();
}

ListTasks::~ListTasks()
{
    delete ui;
}

/**
 * Loads tasks list to tableView.
 */
void ListTasks::loadTable()
{
    queryModel = new QSqlQueryModel(this);

    QString userLogin = global::getSettingsValue("userLogin", "settings").toString();

    QString queryString = "SELECT id_to_do_list, time, date, user FROM tasks_table WHERE user = " + userLogin;

    queryModel->setQuery(queryString);

    queryModel->setHeaderData(0, Qt::Horizontal, tr("id"));
    queryModel->setHeaderData(1, Qt::Horizontal, tr("Время"));
    queryModel->setHeaderData(2, Qt::Horizontal, tr("Дата"));
    queryModel->insertColumn(3);
    queryModel->setHeaderData(3, Qt::Horizontal, tr("Содержание"));
    queryModel->insertColumn(4);
    queryModel->setHeaderData(4, Qt::Horizontal, tr("Выполнено"));
    queryModel->setHeaderData(5, Qt::Horizontal, tr("Пользователь"));

    ui->tableView->setModel(queryModel);

    ui->tableView->setColumnHidden(0, true);
    ui->tableView->setColumnHidden(5, true);

    for (int rowIndex = 0; rowIndex < ui->tableView->model()->rowCount(); ++rowIndex)
    {
        ui->tableView->setIndexWidget(queryModel->index(rowIndex, 3), addWidgetContent(rowIndex));
        ui->tableView->setIndexWidget(queryModel->index(rowIndex, 4), addCheckBoxCompleted(rowIndex));
    }

    ui->tableView->resizeRowsToContents();
    ui->tableView->resizeColumnsToContents();
    ui->tableView->horizontalHeader()->setDefaultSectionSize(maximumWidth());
    ui->tableView->setColumnWidth(3, 550);
}

/**
 * Adds label widget for content dispalying in tableView.
 */
QWidget* ListTasks::addWidgetContent(int rowIndex)
{
    QString userLogin = global::getSettingsValue("userLogin", "settings").toString();

    QWidget *widget = new QWidget(this);
    QHBoxLayout *layout = new QHBoxLayout(widget);
    QLabel *taskContentLabel = new QLabel(widget);

    layout->addWidget(taskContentLabel);

    queryModelLabel = new QSqlQueryModel(this);

    QString queryString = "SELECT content FROM tasks_table WHERE user = " + userLogin;

    queryModelLabel->setQuery(queryString);

    QString taskContent = queryModelLabel->data(queryModelLabel->index(rowIndex, 0), Qt::EditRole).toString();

    taskContentLabel->setWordWrap(true);
    taskContentLabel->setText(taskContent);
    taskContentLabel->setOpenExternalLinks(true);

    return widget;
}

/**
 * Adds checkBox widget to tableView.
 * It determines whether the task is completed or not.
 */
QWidget* ListTasks::addCheckBoxCompleted(int rowIndex)
{
    QString userLogin = global::getSettingsValue("userLogin", "settings").toString();

    QWidget *widget = new QWidget(this);
    QHBoxLayout *layout = new QHBoxLayout(widget);
    QCheckBox *checkBox = new QCheckBox(widget);

    layout->addWidget(checkBox, 0, Qt::AlignCenter);

    queryModelCheckBox = new QSqlQueryModel(this);

    QString queryStringCheckBox = "SELECT is_fulfilled FROM tasks_table WHERE user = " + userLogin;

    queryModelCheckBox->setQuery(queryStringCheckBox);

    QString isFulfilled = queryModelCheckBox->data(queryModelCheckBox->index(rowIndex, 0), Qt::EditRole).toString();

    // set checked/unchecked in tableView
    if (isFulfilled == "1")
        checkBox->setChecked(true);
    else
        checkBox->setChecked(false);

    connect(checkBox, &QAbstractButton::pressed, this, &ListTasks::checkBoxStateChanged);

    QString id = queryModel->data(queryModel->index(rowIndex, 0), Qt::EditRole).toString();

    checkBox->setProperty("checkBox", QVariant::fromValue(checkBox));
    checkBox->setProperty("id",       QVariant::fromValue(id));

    return widget;
}

/**
 * Processes checkBox state change.
 */
void ListTasks::checkBoxStateChanged()
{
    QString id = sender()->property("id").value<QString>();
    QCheckBox *checkBox = sender()->property("checkBox").value<QCheckBox*>();

    QSqlQuery query(listTasksTable);

    if (!checkBox->isChecked())
    {
        checkBox->setChecked(true);

        query.prepare("UPDATE tasks_table SET is_fulfilled = 1 WHERE id_to_do_list = ?");
        query.addBindValue(id);
        query.exec();
    }
    else if (checkBox->isChecked())
    {
        checkBox->setChecked(false);

        query.prepare("UPDATE tasks_table SET is_fulfilled = 0 WHERE id_to_do_list = ?");
        query.addBindValue(id);
        query.exec();
    }

    on_updateButton_clicked();
}

/**
 * Opens AddSparePart window.
 */
void ListTasks::on_addTaskButton_clicked()
{
    QDialog::close();

    addTask = new AddTask;
    addTask->show();
    addTask->setAttribute(Qt::WA_DeleteOnClose);
}

/**
 * Opens ViewUpdateSparePart window.
 */
void ListTasks::showTaskInfo(const QModelIndex &index)
{
    QDialog::close();

    QString taskId = queryModel->data(queryModel->index(index.row(), 0)).toString();

    viewUpdateTask = new ViewUpdateTask;
    viewUpdateTask->setValues(taskId);
    viewUpdateTask->show();
    viewUpdateTask->setAttribute(Qt::WA_DeleteOnClose);
}

void ListTasks::on_updateButton_clicked()
{
   queryModel->setQuery(NULL);

   loadTable();
}
