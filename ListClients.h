#ifndef LISTCLIENTS_H
#define LISTCLIENTS_H

#include "AddClient.h"
#include "ViewClient.h"

#include <QDialog>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QPointer>
#include <QSqlQuery>
#include <QDebug>
#include <QDir>

class ViewClient;

namespace Ui {
class ListClients;
}

class ListClients : public QDialog
{
    Q_OBJECT

public:
    explicit ListClients(QWidget *parent = nullptr);
    ~ListClients();

private slots:
    void loadTable();
    void showClientInfo(const QModelIndex &index);

    void on_addClientButton_clicked();

private:
    Ui::ListClients *ui;

    AddClient *addClient;
    ViewClient *viewClient;

    QSqlDatabase clientsTable = QSqlDatabase::database("ClientsTable");
    QSqlDatabase clientsDB;
    QPointer<QSqlQueryModel> queryModel;
};

#endif // LISTCLIENTS_H
