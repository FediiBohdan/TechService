#ifndef VIEWORDERS_H
#define VIEWORDERS_H

#include <QDialog>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QPointer>
#include <QSqlQuery>
#include <QDebug>

namespace Ui {
class ViewOrders;
}

class ViewOrders : public QDialog
{
    Q_OBJECT

public:
    explicit ViewOrders(QWidget *parent = nullptr);
    ~ViewOrders();

    void setValues(const QString &id);

private:
    Ui::ViewOrders *ui;

    QSqlDatabase ordersTable = QSqlDatabase::database("OrdersHistory");
    QSqlDatabase ordersDB;

    QString orderId;
};

#endif // VIEWORDERS_H
