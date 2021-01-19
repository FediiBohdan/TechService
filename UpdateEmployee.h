#ifndef UPDATEEMPLOYEE_H
#define UPDATEEMPLOYEE_H

#include "ViewEmployee.h"

#include <QDialog>
#include <QSqlDatabase>

class ViewEmployee;

namespace Ui {
class UpdateEmployee;
}

class UpdateEmployee : public QDialog
{
    Q_OBJECT

signals:
    void sendData(bool update);

public:
    explicit UpdateEmployee(QWidget *parent = nullptr);
    ~UpdateEmployee();

public slots:
    void setValues(const QString &id);

private slots:
    void on_backToViewInfoButton_clicked();
    void on_saveUpdatedInfo_clicked();

private:
    Ui::UpdateEmployee *ui;

    QSqlDatabase employeesDB;

    QString employeeId;
};

#endif // UPDATEEMPLOYEE_H
