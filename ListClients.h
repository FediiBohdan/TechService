#ifndef LISTCLIENTS_H
#define LISTCLIENTS_H

#include "AddClient.h"

#include <QDialog>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QPointer>
#include <QSqlQuery>
#include <QDebug>

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

    void on_addClientButton_clicked();

private:
    Ui::ListClients *ui;

    AddClient *addClient;

    QSqlDatabase clientsTable = QSqlDatabase::database("ClientsTable");
    QSqlDatabase clientsDB;
    QPointer<QSqlQueryModel> queryModel;
};

#endif // LISTCLIENTS_H
