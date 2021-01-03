#ifndef ORDERCREATION_H
#define ORDERCREATION_H

#include "StaffTable.h"
#include "SparePartsTable.h"

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

class StartWindow;
class SparePartsTable;
class OrdersHistory;

namespace Ui {
class OrderCreation;
}

class OrderCreation : public QDialog
{
    Q_OBJECT

public:
    explicit OrderCreation(QWidget *parent = nullptr);
    ~OrderCreation();

private:
    Ui::OrderCreation *ui;

    SparePartsTable *sparePartsTable;

    QSqlDatabase ordersHistory = QSqlDatabase::database("OrdersHistory");
    QSqlDatabase ordersHistoryDB;
    QPointer<QSqlQueryModel> queryModel;

private slots:
    void openMap();
    void setDateAndTime();

    void on_createOrderButton_clicked();

public slots:
    void closeWindow();
};

#endif // ORDERCREATION_H
