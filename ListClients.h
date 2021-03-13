#ifndef LISTCLIENTS_H
#define LISTCLIENTS_H

#include "AddClient.h"
#include "ViewUpdateClient.h"

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

class ViewUpdateClient;
class AddClient;

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
    void saveAsCSV(QString fileName);
    void showClientInfo(const QModelIndex &index);

    void on_addClientButton_clicked();
    void on_clientSearch_returnPressed();
    void on_updateButton_clicked();
    void on_csvExportButton_clicked();

private:
    Ui::ListClients *ui;

    AddClient *addClient;
    ViewUpdateClient *viewUpdateClient;

    bool searchFlag;

    QSqlDatabase clientsTable = QSqlDatabase::database("ClientsTable");
    QSqlDatabase clientsDB;
    QPointer<QSqlQueryModel> queryModel;
};

#endif // LISTCLIENTS_H
