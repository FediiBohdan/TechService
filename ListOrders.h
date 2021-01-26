#ifndef LISTORDERS_H
#define LISTORDERS_H

#include "AddOrder.h"
#include "ViewOrders.h"

#include <QDialog>
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QPointer>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QDebug>
#include <QCheckBox>

class AddOrder;
class ViewOrders;

namespace Ui {
class ListOrders;
}

class ListOrders : public QDialog
{
    Q_OBJECT

public:
    explicit ListOrders(QWidget *parent = nullptr);
    ~ListOrders();

private slots:
    void loadTable();
    void showOrderInfo(const QModelIndex &index);
    void checkBoxStateChanged();

    void on_orderCreationButton_clicked();
    void on_updateButton_clicked();
    void on_orderSearch_returnPressed();

private:
    Ui::ListOrders *ui;

    QSqlDatabase ordersHistoryTable = QSqlDatabase::database("OrdersHistory");

    QSqlDatabase ordersHistoryDB;
    QPointer<QSqlQueryModel> queryModel;
    QPointer<QSqlQueryModel> queryModelCheckBox;

    AddOrder *addOrder;
    ViewOrders *viewOrders;

    bool searchFlag;

    QWidget *addCheckBoxCompleted(int rowIndex);
};

#endif // LISTORDERS_H
