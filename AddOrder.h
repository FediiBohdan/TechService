#ifndef ADDORDER_H
#define ADDORDER_H

#include "ListEmployees.h"
#include "ListSpareParts.h"
#include "ListOrders.h"

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
#include <QLabel>

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

public slots:
    void closeWindow();

private:
    Ui::AddOrder *ui;

    ListSparePart *sparePartsTable;

    QSqlDatabase listOrdersTable = QSqlDatabase::database("OrdersHistory");
    QSqlDatabase listSparePartsTable = QSqlDatabase::database("SparePartsCatalogue");

    QSqlDatabase ordersHistoryDB;
    QSqlDatabase clientsDB;
    QSqlDatabase sparePartsDB;

    QPointer<QSqlQueryModel> queryModel;
    QPointer<QSqlQueryModel> querySparePartsModel;
    QPointer<QSqlQueryModel> queryModelLabel;

    ListOrders *listOrders;

    QWidget *addWidgetCompatibilityContent(int row_index);

private slots:
    void openMap();
    void setDateAndTime();
    void loadSparePartsTable();

    void on_createOrderButton_clicked();

    void closeEvent(QCloseEvent *);
};

#endif // ADDORDER_H
