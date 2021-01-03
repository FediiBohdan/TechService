#ifndef ORDERSHISTORY_H
#define ORDERSHISTORY_H

#include "OrderCreation.h"

#include <QDialog>
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QPointer>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QDebug>

class OrderCreation;

namespace Ui {
class OrdersHistory;
}

class OrdersHistory : public QDialog
{
    Q_OBJECT

public:
    explicit OrdersHistory(QWidget *parent = nullptr);
    ~OrdersHistory();

private slots:
    void loadTable();

    void on_orderCreationButton_clicked();

private:
    Ui::OrdersHistory *ui;

    QSqlDatabase ordersHistoryTable = QSqlDatabase::database("OrdersHistory");

    QSqlDatabase ordersHistoryDB;
    QPointer<QSqlQueryModel> queryModel;

    OrderCreation *orderCreation;
};

#endif // ORDERSHISTORY_H
