#ifndef VIEWORDERS_H
#define VIEWORDERS_H

#include "ListOrders.h"
#include "UpdateOrders.h"

#include <QDialog>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QPointer>
#include <QSqlQuery>
#include <QDebug>

class ListOrders;
class UpdateOrders;

namespace Ui {
class ViewOrders;
}

class ViewOrders : public QDialog
{
    Q_OBJECT

public:
    explicit ViewOrders(QWidget *parent = nullptr);
    ~ViewOrders();

public slots:
    void setValues(const QString &id);
    void receiveData(bool update);

private:
    Ui::ViewOrders *ui;

    ListOrders *listOrders;
    UpdateOrders *updateOrders;

    QSqlDatabase ordersDB;

    QString orderId;

private slots:
    void closeEvent(QCloseEvent*);
    void on_updateOrderInfoButton_clicked();
};

#endif // VIEWORDERS_H
