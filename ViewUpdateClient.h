#ifndef VIEWUPDATECLIENT_H
#define VIEWUPDATECLIENT_H

#include "ListClients.h"
#include "AddOrder.h"

#include <QDialog>
#include <QSqlDatabase>

class ListClients;
class AddOrder;

namespace Ui {
class ViewUpdateClient;
}

class ViewUpdateClient : public QDialog
{
    Q_OBJECT

signals:
    void sendData(bool update);

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

    QString clientId;

    ListClients *listClients;
};

#endif // VIEWUPDATECLIENT_H
