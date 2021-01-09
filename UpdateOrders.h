#ifndef UPDATEORDERS_H
#define UPDATEORDERS_H

#include "ViewOrders.h"

#include <QDialog>
#include <QSqlDatabase>

class ViewOrders;

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
    void setValues(const QString& id);

private slots:
    void on_backToViewInfoButton_clicked();

    void on_saveUpdatedInfo_clicked();

private:
    Ui::UpdateOrders *ui;

    QSqlDatabase ordersDB;

    QString orderId;
};

#endif // UPDATEORDERS_H
