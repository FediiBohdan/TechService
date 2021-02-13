#ifndef UPDATECLIENTS_H
#define UPDATECLIENTS_H

#include "ViewClient.h"
#include "ListClients.h"

#include <QDialog>
#include <QSqlDatabase>

class ViewClient;
class ListClients;

namespace Ui {
class UpdateClients;
}

class UpdateClients : public QDialog
{
    Q_OBJECT

signals:
    void sendData(bool update);

public:
    explicit UpdateClients(QWidget *parent = nullptr);
    ~UpdateClients();

public slots:
    void setValues(const QString &id);

private slots:
    void closeEvent(QCloseEvent *);

    void on_saveUpdatedInfo_clicked();
    void on_backToViewInfoButton_clicked();

private:
    Ui::UpdateClients *ui;

    QSqlDatabase clientsDB;

    QString clientId;

    ListClients *listClients;
};

#endif // UPDATECLIENTS_H
