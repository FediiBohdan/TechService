#ifndef VIEWUPDATEORDER_H
#define VIEWUPDATEORDER_H

#include "ListOrders.h"

#include <QDir>
#include <QDate>
#include <QTime>
#include <QDebug>
#include <QLabel>
#include <QDialog>
#include <QPointer>
#include <QSqlQuery>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQueryModel>

class ListOrders;

namespace Ui {
class ViewUpdateOrder;
}

class ViewUpdateOrder : public QDialog
{
    Q_OBJECT

signals:
    void sendData(bool update);

public:
    explicit ViewUpdateOrder(QWidget *parent = nullptr);
    ~ViewUpdateOrder();

public slots:
    void setValues(const QString &id);

private slots:
    void setDateAndTime();    
    void loadEmployeesTable();
    void loadSparePartsTable();
    void updateEmployeesTable();
    void updateSparePartsTable();
    void loadUsedSparePartsTable();
    void updateUsedSparePartsTable();
    void updateAvailableSparePartsTable();
    void setOrderEmployees(const QModelIndex &index);
    void removeUsedSparePartsTable(const QModelIndex &index);
    void updateUsedSparePartsListTable(const QModelIndex &index);

    void closeEvent(QCloseEvent *);

    void on_saveUpdatedInfo_clicked();
    void on_deleteOrderButton_clicked();
    void on_clearWasherButton_clicked();
    void on_clearMechanicButton_clicked();
    void on_clearLocksmithButton_clicked();
    void on_clearMechanic2Button_clicked();
    void on_updateOrderInfoButton_clicked();
    void on_clearElectronicButton_clicked();
    void on_sparePartsSearch_returnPressed();
    void on_clearDiagnosticianButton_clicked();

private:
    Ui::ViewUpdateOrder *ui;

    ListOrders *listOrders;

    QSqlDatabase listOrdersTable = QSqlDatabase::database("OrdersHistory");
    QSqlDatabase orderDetailTable = QSqlDatabase::database("OrderDetailTable");
    QSqlDatabase listSparePartsTable = QSqlDatabase::database("SparePartsCatalogue");

    QSqlDatabase ordersDB;
    QSqlDatabase clientsDB;
    QSqlDatabase employeesDB;
    QSqlDatabase sparePartsDB;
    QSqlDatabase orderDetailDB;
    QSqlDatabase ordersHistoryDB;

    QPointer<QSqlQueryModel> queryModel;
    QPointer<QSqlQueryModel> queryModelLabel;
    QPointer<QSqlQueryModel> queryEmployeesModel;
    QPointer<QSqlQueryModel> queryEmployeesHoursModel;
    QPointer<QSqlQueryModel> queryRemoveEmployeeModel;
    QPointer<QSqlQueryModel> queryUsedSparePartsModel;
    QPointer<QSqlQueryModel> queryUpdateSparePartsModel;
    QPointer<QSqlQueryModel> queryGetUsedSparePartsModel;
    QPointer<QSqlQueryModel> queryAvailableSparePartsModel;
    QPointer<QSqlQueryModel> queryRemoveUsedSparePartsModel;

    bool searchFlag;

    QString orderId;

    int washerHourPayment = 0;
    int mechanicHourPayment = 0;
    int locksmithHourPayment = 0;
    int mechanic2HourPayment = 0;
    int totalEmployeePayment = 0;
    int electronicHourPayment = 0;
    int diagnosticianHourPayment = 0;
    int couponDiscount = 3;//%
    int promotionDiscount = 4;//%
    int regularCustomerDiscount = 6;//%
    int specialCondidtionsDiscount = 8;//%    
};

#endif // VIEWUPDATEORDER_H
