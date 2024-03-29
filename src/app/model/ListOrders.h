#ifndef LISTORDERS_H
#define LISTORDERS_H

#include "AddOrder.h"
#include "StartWindow.h"
#include "ViewUpdateOrder.h"

#include <QFile>
#include <QLabel>
#include <QDebug>
#include <QDialog>
#include <QPointer>
#include <QTextStream>
#include <QFileDialog>
#include <QMessageBox>
#include <QSqlQueryModel>

class AddOrder;
class StartWindow;
class ViewUpdateOrder;

namespace Ui {
class ListOrders;
}

class ListOrders : public QDialog
{
    Q_OBJECT

public:
    explicit ListOrders(QWidget *parent = nullptr);
    ~ListOrders();

private slots:
    void loadTable();
    void loadUserSettings();
    void checkBoxStateChanged();
    void saveAsCSV(QString fileName);
    void showOrderInfo(const QModelIndex &index);

    void on_updateButton_clicked();    
    void on_csvExportButton_clicked();
    void on_orderSearch_returnPressed();
    void on_orderCreationButton_clicked();

private:
    Ui::ListOrders *ui;

    QSqlDatabase ordersHistoryTable;
    QPointer<QSqlQueryModel> queryModel;
    QPointer<QSqlQueryModel> queryModelCheckBox;

    AddOrder *addOrder;
    ViewUpdateOrder *viewUpdateOrder;

    bool searchFlag;

    QWidget *addCheckBoxCompleted(int rowIndex);
};

#endif // LISTORDERS_H
