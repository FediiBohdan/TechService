#ifndef ADDORDER_H
#define ADDORDER_H

#include "ListEmployees.h"
#include "ListSpareParts.h"

#include <QDialog>
#include <QQmlApplicationEngine>
#include <QDebug>
#include <QDate>
#include <QTime>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QPointer>
#include <QSqlQuery>
#include <QDir>

class StartWindow;
class ListSparePart;
class ListOrders;

namespace Ui {
class AddOrder;
}

class AddOrder : public QDialog
{
    Q_OBJECT

public:
    explicit AddOrder(QWidget *parent = nullptr);
    ~AddOrder();

private:
    Ui::AddOrder *ui;

    ListSparePart *sparePartsTable;

    QSqlDatabase listOrders = QSqlDatabase::database("OrdersHistory");
    QSqlDatabase ordersHistoryDB;
    QSqlDatabase clientsDB;
    QPointer<QSqlQueryModel> queryModel;

private slots:
    void openMap();
    void setDateAndTime();

    void on_createOrderButton_clicked();

public slots:
    void closeWindow();
};

#endif // ADDORDER_H
