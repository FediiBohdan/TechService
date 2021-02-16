#ifndef LISTCLIENTS_H
#define LISTCLIENTS_H

#include "AddClient.h"
#include "ViewClient.h"
#include "UpdateClients.h"

#include <QDialog>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QPointer>
#include <QSqlQuery>
#include <QDebug>
#include <QDir>
#include <QLabel>
#include <QFile>
#include <QTextStream>
#include <QFileDialog>

class ViewClient;
class AddClient;
class UpdateClients;

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
    void saveAsCSV(QString filename);
    void showClientInfo(const QModelIndex &index);

    void on_addClientButton_clicked();
    void on_clientSearch_returnPressed();
    void on_updateButton_clicked();
    void on_csvExportButton_clicked();

private:
    Ui::ListClients *ui;

    AddClient *addClient;
    ViewClient *viewClient;

    bool searchFlag;

    QSqlDatabase clientsTable = QSqlDatabase::database("ClientsTable");
    QSqlDatabase clientsDB;
    QPointer<QSqlQueryModel> queryModel;
};

#endif // LISTCLIENTS_H
