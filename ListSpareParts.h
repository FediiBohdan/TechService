#ifndef ListSpareParts_H
#define ListSpareParts_H

#include "StartWindow.h"
#include "AddSparePart.h"
#include "ViewSparePart.h"

#include <QDialog>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QPointer>
#include <QSqlQuery>
#include <QDebug>

class StartWindow;

namespace Ui {
class ListSparePart;
}

class ListSparePart : public QDialog
{
    Q_OBJECT

public:
    explicit ListSparePart(QWidget *parent = nullptr);
    ~ListSparePart();

private:
    Ui::ListSparePart *ui;

    QSqlDatabase listSpareParts = QSqlDatabase::database("SparePartsCatalogue");

    QSqlDatabase listSparePartsDB;
    QPointer<QSqlQueryModel> queryModel;

    AddSparePart *addSparePart;
    StartWindow *startWindow;
    ViewSparePart *viewSparePart;

private slots:
    void loadTable();
    void showSparePartInfo(const QModelIndex &index);

    void on_addSparePartButton_clicked();

public slots:
    void closeWindow();
};

#endif // ListSpareParts_H
