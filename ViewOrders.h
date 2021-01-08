#ifndef VIEWORDERS_H
#define VIEWORDERS_H

#include "ListOrders.h"

#include <QDialog>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QPointer>
#include <QSqlQuery>
#include <QDebug>

class ListOrders;

namespace Ui {
class ViewOrders;
}

class ViewOrders : public QDialog
{
    Q_OBJECT

public:
    explicit ViewOrders(QWidget *parent = nullptr);
    ~ViewOrders();

public slots:
    void setValues(const QString &id);

private:
    Ui::ViewOrders *ui;

    ListOrders *listOrders;

    QSqlDatabase ordersDB;

    QString orderId;

private slots:
    void closeEvent(QCloseEvent*);
};

#endif // VIEWORDERS_H
