#ifndef VIEWCLIENT_H
#define VIEWCLIENT_H

#include "ListClients.h"
#include "UpdateClients.h"

#include <QDialog>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QPointer>
#include <QSqlQuery>
#include <QDebug>

class ListClients;
class UpdateClients;

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
    void setValues(const QString &id);
    void receiveData(bool update);

private:
    Ui::ViewClient *ui;

    ListClients *listClient;
    UpdateClients *updateClients;

    QSqlDatabase clientsDB;
    QString clientId;

private slots:
    void closeEvent(QCloseEvent*);

    void on_updateClientInfoButton_clicked();
};

#endif // VIEWCLIENT_H
