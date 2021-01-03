#ifndef ADDWORKER_H
#define ADDWORKER_H

#include <QDialog>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QPointer>
#include <QSqlQuery>
#include <QDebug>

namespace Ui {
class AddWorker;
}

class AddWorker : public QDialog
{
    Q_OBJECT

public:
    explicit AddWorker(QWidget *parent = nullptr);
    ~AddWorker();

private slots:
    void on_saveWorkerButton_clicked();

private:
    Ui::AddWorker *ui;

    QSqlDatabase staffTable = QSqlDatabase::database("StaffTable");
    QSqlDatabase staffDB;
    QPointer<QSqlQueryModel> queryModel;
};

#endif // ADDWORKER_H
