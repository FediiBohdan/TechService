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
    void setValues(const QString &clientName, const QString &clientContacts, const QString &autoBrand);

private:
    Ui::AddOrder *ui;

    ListSparePart *sparePartsTable;

    QSqlDatabase listOrdersTable = QSqlDatabase::database("OrdersHistory");
    QSqlDatabase listSparePartsTable = QSqlDatabase::database("SparePartsCatalogue");
    QSqlDatabase orderDetailTable = QSqlDatabase::database("OrderDetailTable");

    QSqlDatabase clientsDB;
    QSqlDatabase sparePartsDB;
    QSqlDatabase orderDetailDB;
    QSqlDatabase ordersHistoryDB;
    QSqlDatabase ordersHistoryDB1;

    QPointer<QSqlQueryModel> queryModel;
    QPointer<QSqlQueryModel> queryAvailableSparePartsModel;
    QPointer<QSqlQueryModel> queryGetUsedSparePartsModel;
    QPointer<QSqlQueryModel> queryUpdateSparePartsModel;
    QPointer<QSqlQueryModel> queryUsedSparePartsModel;
    QPointer<QSqlQueryModel> queryRemoveUsedSparePartsModel;
    QPointer<QSqlQueryModel> queryModelLabel;
    QPointer<QSqlQueryModel> queryEmployeesModel;
    QPointer<QSqlQueryModel> queryEmployeesHoursModel;

    ListOrders *listOrders;

    bool searchFlag;
    bool openFlag = true;
    QString sparePartsList;
    QString s_orderId;
    float sparePartCost;
    float sparePartsCost = 0;
    int orderId = 0;
    int sparePartNameLength = 0;
    int mechanicHourPayment = 0;
    int mechanic2HourPayment = 0;
    int diagnosticianHourPayment = 0;
    int electronicHourPayment = 0;
    int locksmithHourPayment = 0;
    int washerHourPayment = 0;
    int totalEmployeePayment = 0;
    int couponDiscount = 3;//%
    int promotionDiscount = 4;//%
    int specialCondidtionsDiscount = 8;//%
    int regularCustomerDiscount = 6;//%

private slots:
    void openMap();
    void setDateAndTime();
    void loadSparePartsTable();    
    void loadEmployeesTable();
    void loadUsedSparePartsTable();
    void updateSparePartsTable();
    void updateEmployeesTable();
    void updateusedSparePartsTable();
    void updateUsedSparePartsTable(const QModelIndex &index);
    void removeUsedSparePartsTable(const QModelIndex &index);
    void setOrderEmployees(const QModelIndex &index);

    void closeEvent(QCloseEvent *);

    void on_createOrderButton_clicked();
    void on_sparePartsSearch_returnPressed();
    void on_clearMechanicButton_clicked();
    void on_clearMechanic2Button_clicked();
    void on_clearDiagnosticianButton_clicked();
    void on_clearElectronicButton_clicked();
    void on_clearWasherButton_clicked();
    void on_clearLocksmithButton_clicked();
    void on_addSparePartsButton_clicked();
};

#endif // ADDORDER_H
