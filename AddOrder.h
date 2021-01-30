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
    QSqlDatabase testDB;
    QSqlDatabase clientsDB;
    QSqlDatabase sparePartsDB;

    QPointer<QSqlQueryModel> queryModel;
    QPointer<QSqlQueryModel> queryAvailableSparePartsModel;
    QPointer<QSqlQueryModel> queryUpdateSparePartsModel;
    QPointer<QSqlQueryModel> queryUsedSparePartsModel;
    QPointer<QSqlQueryModel> queryModelLabel;
    QPointer<QSqlQueryModel> queryEmployeesModel;
    QPointer<QSqlQueryModel> queryEmployeesHoursModel;

    ListOrders *listOrders;

    QWidget *addWidgetCompatibilityContent(int rowIndex);
    QWidget *addWidgetHoursLine(int rowIndex);

    bool searchFlag;
    QString sparePartsList;
    int sparePartCost;
    int sparePartsCost = 0;

private slots:
    void openMap();
    void setDateAndTime();
    void loadSparePartsTable();
    void loadEmployeesTable();
    void updateSparePartsTable();
    void updateUsedSparePartsTable(const QModelIndex &index);

    void on_createOrderButton_clicked();
    void on_sparePartsSearch_returnPressed();
    void on_clearSparePartsListButton_clicked();

    void closeEvent(QCloseEvent *);
};

#endif // ADDORDER_H
