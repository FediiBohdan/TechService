#ifndef ADDCLIENT_H
#define ADDCLIENT_H

#include "ListClients.h"

#include <QDebug>
#include <QDialog>
#include <QPointer>
#include <QSqlQuery>
#include <QMessageBox>
#include <QSqlDatabase>

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

    QSqlDatabase clientsTable;
};

#endif // ADDCLIENT_H
