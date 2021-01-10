#include "TaskList.h"
#include "ui_TaskList.h"

TaskList::TaskList(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TaskList)
{
    ui->setupUi(this);

    ui->completedListView->setDragEnabled(true);
    ui->completedListView->setAcceptDrops(true);
    ui->completedListView->setDropIndicatorShown(true);
    ui->completedListView->setDefaultDropAction(Qt::MoveAction);

    ui->notCompletedListView->setDragEnabled(true);
    ui->notCompletedListView->setAcceptDrops(true);
    ui->notCompletedListView->setDropIndicatorShown(true);
    ui->notCompletedListView->setDefaultDropAction(Qt::MoveAction);

    ui->completedListView->setModel(new QStringListModel());
    ui->notCompletedListView->setModel(new QStringListModel());

    connect(ui->addButton, &QAbstractButton::clicked, this, &TaskList::onAdd);
    connect(ui->removeButton, &QAbstractButton::clicked, this, &TaskList::onRemove);
}

TaskList::~TaskList()
{
    delete ui;
}

void TaskList::onAdd()
{
    ui->notCompletedListView->model()->insertRow(ui->notCompletedListView->model()->rowCount());
    QModelIndex qIndex = ui->notCompletedListView->model()->index(ui->notCompletedListView->model()->rowCount() - 1, 0);

    ui->notCompletedListView->edit(qIndex);
}

void TaskList::onRemove()
{
    QModelIndex qIndex = ui->notCompletedListView->currentIndex();
    ui->notCompletedListView->model()->removeRow(qIndex.row());
}
