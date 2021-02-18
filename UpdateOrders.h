#ifndef UPDATEORDERS_H
#define UPDATEORDERS_H

#include "ViewOrders.h"
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

class ViewOrders;
class ListOrders;

namespace Ui {
class UpdateOrders;
}

class UpdateOrders : public QDialog
{
    Q_OBJECT

signals:
    void sendData(bool update);

public:
    explicit UpdateOrders(QWidget *parent = nullptr);
    ~UpdateOrders();

public slots:
    void setValues(const QString &id);

private slots:
    void openMap();
    void setDateAndTime();
    void loadSparePartsTable();
    void loadEmployeesTable();
    void updateSparePartsTable();
    void updateEmployeesTable();
    void updateUsedSparePartsTable(const QModelIndex &index);
    void setOrderEmployees(const QModelIndex &index);

    void closeEvent(QCloseEvent *);

    void on_sparePartsSearch_returnPressed();
    void on_clearSparePartsListButton_clicked();
    void on_clearMechanicButton_clicked();
    void on_clearMechanic2Button_clicked();
    void on_clearDiagnosticianButton_clicked();
    void on_clearElectronicButton_clicked();
    void on_clearWasherButton_clicked();
    void on_clearLocksmithButton_clicked();
    void on_removeLastSparePartButton_clicked();
    void on_backToViewInfoButton_clicked();
    void on_saveUpdatedInfo_clicked();

private:
    Ui::UpdateOrders *ui;

    ListOrders *listOrders;

    QSqlDatabase listOrdersTable = QSqlDatabase::database("OrdersHistory");
    QSqlDatabase listSparePartsTable = QSqlDatabase::database("SparePartsCatalogue");
    QSqlDatabase orderDetailTable = QSqlDatabase::database("OrderDetailTable");

    QSqlDatabase ordersDB;
    QSqlDatabase orderDetailDB;
    QSqlDatabase clientsDB;
    QSqlDatabase sparePartsDB;
    QSqlDatabase ordersHistoryDB;

    QPointer<QSqlQueryModel> queryModel;
    QPointer<QSqlQueryModel> queryAvailableSparePartsModel;
    QPointer<QSqlQueryModel> queryUpdateSparePartsModel;
    QPointer<QSqlQueryModel> queryUsedSparePartsModel;
    QPointer<QSqlQueryModel> queryModelLabel;
    QPointer<QSqlQueryModel> queryEmployeesModel;
    QPointer<QSqlQueryModel> queryEmployeesHoursModel;

    QWidget *addWidgetCompatibilityContent(int rowIndex);

    bool searchFlag;
    bool mechanicFlag;
    bool mechanic2Flag;
    bool diagnosticianFlag;
    bool electronicFlag;
    bool locksmithFlag;
    bool washerFlag;

    QString orderId;
    QString sparePartsList;

    float sparePartCost;
    float sparePartsCost = 0;

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
};

#endif // UPDATEORDERS_H
