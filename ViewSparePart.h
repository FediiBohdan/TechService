#ifndef VIEWSPAREPART_H
#define VIEWSPAREPART_H

#include "ListSpareParts.h"
#include "UpdateSparePart.h"

#include <QDialog>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QPointer>
#include <QSqlQuery>
#include <QDebug>

class ListSparePart;
class UpdateSparePart;

namespace Ui {
class ViewSparePart;
}

class ViewSparePart : public QDialog
{
    Q_OBJECT

public:
    explicit ViewSparePart(QWidget *parent = nullptr);
    ~ViewSparePart();

public slots:
    void setValues(const QString &id);
    void receiveData(bool update);

private:
    Ui::ViewSparePart *ui;

    ListSparePart *listSparePart;
    UpdateSparePart *updateSparePart;

    QSqlDatabase sparePartsDB;

    QString sparePartId;

private slots:
    void closeEvent(QCloseEvent *);
    void on_updateSparePartInfoButton_clicked();
};

#endif // VIEWSPAREPART_H
