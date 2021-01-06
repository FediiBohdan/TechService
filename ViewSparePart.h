#ifndef VIEWSPAREPART_H
#define VIEWSPAREPART_H

#include <QDialog>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QPointer>
#include <QSqlQuery>
#include <QDebug>


namespace Ui {
class ViewSparePart;
}

class ViewSparePart : public QDialog
{
    Q_OBJECT

public:
    explicit ViewSparePart(QWidget *parent = nullptr);
    ~ViewSparePart();

    void setValues(const QString &id);

private:
    Ui::ViewSparePart *ui;

    QSqlDatabase sparePartsTable = QSqlDatabase::database("SparePartsCatalogue");
    QSqlDatabase sparePartsDB;

    QString sparePartId;
};

#endif // VIEWSPAREPART_H
