#ifndef TASKLIST_H
#define TASKLIST_H

#include <QDialog>
#include <QListView>
#include <QStringListModel>

namespace Ui {
class TaskList;
}

class TaskList : public QDialog
{
    Q_OBJECT

public:
    explicit TaskList(QWidget *parent = nullptr);
    ~TaskList();
    void CToDoList();

protected slots:
    void onAdd();
    void onRemove();

private:
    Ui::TaskList *ui;

};

#endif // TASKLIST_H
