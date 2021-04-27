#ifndef VIEWUPDATECLIENT_H
#define VIEWUPDATECLIENT_H

#include "AddOrder.h"
#include "ListClients.h"

#include <QDialog>
#include <QSqlDatabase>

class AddOrder;
class ListClients;

namespace Ui {
class ViewUpdateClient;
}

class ViewUpdateClient : public QDialog
{
    Q_OBJECT

signals:
    void sendData(bool update);
    void sendValues(QString clientName, QString contacts, QString autoBrand);

public:
    explicit ViewUpdateClient(QWidget *parent = nullptr);
    ~ViewUpdateClient();

public slots:
    void setValues(const QString &id);

private slots:
    void closeEvent(QCloseEvent *);

    void loadUserSettings();
    void loadOrderHistoryTable();

    void on_saveUpdatedInfo_clicked();
    void on_updateInfoButton_clicked();    
    void on_deleteClientButton_clicked();
    void on_createOrderByClientButton_clicked();

private:
    Ui::ViewUpdateClient *ui;

    QSqlDatabase clientsTable;
    QPointer<QSqlQueryModel> queryModel;

    AddOrder *addOrder;
    ListClients *listClients;

    QString clientId;
};

#endif // VIEWUPDATECLIENT_H
