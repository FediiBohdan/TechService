#ifndef VIEWCLIENT_H
#define VIEWCLIENT_H

#include <QDialog>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QPointer>
#include <QSqlQuery>
#include <QDebug>

namespace Ui {
class ViewClient;
}

class ViewClient : public QDialog
{
    Q_OBJECT

public:
    explicit ViewClient(QWidget *parent = nullptr);
    ~ViewClient();

    void setValues(const QString &id);

private:
    Ui::ViewClient *ui;

    QSqlDatabase clientsTable = QSqlDatabase::database("ClientsTable");
    QSqlDatabase clientsDB;

    QString clientId;
};

#endif // VIEWCLIENT_H
