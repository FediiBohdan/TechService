#ifndef ADDCLIENT_H
#define ADDCLIENT_H

#include "ListClients.h"

#include <QDialog>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QPointer>
#include <QSqlQuery>
#include <QDebug>
#include <QDir>

class ListClients;

namespace Ui {
class AddClient;
}

class AddClient : public QDialog
{
    Q_OBJECT

public:
    explicit AddClient(QWidget *parent = nullptr);
    ~AddClient();

private slots:
    void on_saveClientButton_clicked();

    void closeEvent(QCloseEvent *);

private:
    Ui::AddClient *ui;

    ListClients *listClients;

    QSqlDatabase listClientsTable = QSqlDatabase::database("ClientsTable");
    QSqlDatabase clientsDB;
    QPointer<QSqlQueryModel> queryModel;
};

#endif // ADDCLIENT_H
