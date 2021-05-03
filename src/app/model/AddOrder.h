#ifndef ADDORDER_H
#define ADDORDER_H

#include "ListOrders.h"
#include "ListEmployees.h"
#include "ListSpareParts.h"

#include <QDate>
#include <QTime>
#include <QLabel>
#include <QDebug>
#include <QDialog>
#include <QPointer>
#include <QSqlQuery>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QQmlApplicationEngine>

class ListOrders;
class StartWindow;
class ListSparePart;

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
    void setValues(const QString &clientName, const QString &clientContacts, const QString &autoBrand);

private:
    Ui::AddOrder *ui;

    ListSparePart *sparePartsTable;

    QSqlDatabase clientsTable;
    QSqlDatabase orderDetailTable;
    QSqlDatabase sparePartsTableDB;
    QSqlDatabase ordersHistoryTable;

    QPointer<QSqlQueryModel> queryEmployeesModel;
    QPointer<QSqlQueryModel> queryUsedSparePartsModel;
    QPointer<QSqlQueryModel> queryGetUsedSparePartsModel;
    QPointer<QSqlQueryModel> queryAvailableSparePartsModel;
    QPointer<QSqlQueryModel> queryRemoveUsedSparePartsModel;

    ListOrders *listOrders;

    bool searchFlag;
    bool openFlag = true;

    QString s_orderId;

    int orderId = 0;
    int washerId = 0;
    int mechanicId = 0;
    int mechanic2Id = 0;    
    int locksmithId = 0;
    int electronicId = 0;
    int diagnosticianId = 0;

    int washerHourPayment = 0;
    int mechanicHourPayment = 0;
    int mechanic2HourPayment = 0;
    int locksmithHourPayment = 0;
    int electronicHourPayment = 0;
    int diagnosticianHourPayment = 0;

    int couponDiscount = 3; //%
    int promotionDiscount = 4; //%
    int regularCustomerDiscount = 6; //%
    int specialCondidtionsDiscount = 8; //%

    int totalEmployeePayment = 0;

private slots:
    void openMap();
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

    void on_createOrderButton_clicked();
    void on_clearWasherButton_clicked();
    void on_addSparePartsButton_clicked();
    void on_clearMechanicButton_clicked();
    void on_clearMechanic2Button_clicked();
    void on_clearLocksmithButton_clicked();
    void on_clearElectronicButton_clicked();
    void on_sparePartsSearch_returnPressed();
    void on_clearDiagnosticianButton_clicked();
};

#endif // ADDORDER_H
