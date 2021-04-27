#ifndef LISTCLIENTS_H
#define LISTCLIENTS_H

#include "AddClient.h"
#include "ViewUpdateClient.h"

#include <QFile>
#include <QLabel>
#include <QDebug>
#include <QDialog>
#include <QPointer>
#include <QTextStream>
#include <QFileDialog>
#include <QMessageBox>
#include <QSqlQueryModel>

class AddClient;
class ViewUpdateClient;

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
    void loadUserSettings();
    void saveAsCSV(QString fileName);
    void showClientInfo(const QModelIndex &index);

    void on_updateButton_clicked();
    void on_csvExportButton_clicked();
    void on_addClientButton_clicked();
    void on_clientSearch_returnPressed();

private:
    Ui::ListClients *ui;

    QPointer<QSqlQueryModel> queryModel;

    bool searchFlag;

    AddClient *addClient;
    ViewUpdateClient *viewUpdateClient;
};

#endif // LISTCLIENTS_H
