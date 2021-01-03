#ifndef ADDCLIENT_H
#define ADDCLIENT_H

#include <QDialog>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QPointer>
#include <QSqlQuery>
#include <QDebug>

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

private:
    Ui::AddClient *ui;

    QSqlDatabase listClients = QSqlDatabase::database("ClientsTable");
    QSqlDatabase clientsDB;
    QPointer<QSqlQueryModel> queryModel;
};

#endif // ADDCLIENT_H
