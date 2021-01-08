#ifndef VIEWCLIENT_H
#define VIEWCLIENT_H

#include "ListClients.h"

#include <QDialog>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QPointer>
#include <QSqlQuery>
#include <QDebug>

class ListClients;

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

private:
    Ui::ViewClient *ui;

    ListClients *listClient;

    QSqlDatabase clientsDB;
    QString clientId;

private slots:
    void closeEvent(QCloseEvent*);
};

#endif // VIEWCLIENT_H
