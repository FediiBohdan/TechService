#ifndef VIEWCLIENT_H
#define VIEWCLIENT_H

#include "ListClients.h"
#include "UpdateClients.h"
#include "AddOrder.h"

#include <QDialog>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QPointer>
#include <QSqlQuery>
#include <QDebug>

class ListClients;
class UpdateClients;
class AddOrder;

namespace Ui {
class ViewClient;
}

class ViewClient : public QDialog
{
    Q_OBJECT

public:
    explicit ViewClient(QWidget *parent = nullptr);
    ~ViewClient();

public slots:
    void loadOrderHistoryTable();

    void setValues(const QString &id);
    void receiveData(bool update);

private:
    Ui::ViewClient *ui;

    ListClients *listClient;
    UpdateClients *updateClients;
    AddOrder *addOrder;

    QPointer<QSqlQueryModel> queryModel;

    QSqlDatabase clientsDB;
    QString clientId;

private slots:
    void closeEvent(QCloseEvent *);

    void on_updateClientInfoButton_clicked();
    void on_createOrderByClientButton_clicked();
};

#endif // VIEWCLIENT_H
