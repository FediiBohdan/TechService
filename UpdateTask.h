#ifndef UPDATETASK_H
#define UPDATETASK_H

#include "ViewTask.h"

#include <QDialog>
#include <QSqlDatabase>

namespace Ui {
class UpdateTask;
}

class UpdateTask : public QDialog
{
    Q_OBJECT

signals:
    void sendData(bool update);

public:
    explicit UpdateTask(QWidget *parent = nullptr);
    ~UpdateTask();

public slots:
    void setValues(const QString &id);

private slots:
    void on_backToViewInfoButton_clicked();
    void on_saveUpdatedInfo_clicked();

private:
    Ui::UpdateTask *ui;

    QSqlDatabase listTasksDB;
    QString taskId;
};

#endif // UPDATETASK_H
