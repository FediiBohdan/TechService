#ifndef VIEWSPAREPART_H
#define VIEWSPAREPART_H

#include "ListSpareParts.h"

#include <QDialog>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QPointer>
#include <QSqlQuery>
#include <QDebug>

class ListSparePart;

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

private:
    Ui::ViewSparePart *ui;

    ListSparePart *listSparePart;

    QSqlDatabase sparePartsDB;

    QString sparePartId;

private slots:
    void closeEvent(QCloseEvent*);
};

#endif // VIEWSPAREPART_H
