#ifndef VIEWUPDATESPAREPART_H
#define VIEWUPDATESPAREPART_H

#include "ListSpareParts.h"

#include <QDialog>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMessageBox>

namespace Ui {
class ViewUpdateSparePart;
}

class ViewUpdateSparePart : public QDialog
{
    Q_OBJECT

signals:
    void sendData(bool update);

public:
    explicit ViewUpdateSparePart(QWidget *parent = nullptr);
    ~ViewUpdateSparePart();

public slots:
    void setValues(const QString &id);

private slots:
    void on_saveUpdatedInfo_clicked();
    void on_updateSparePartInfoButton_clicked();

    void closeEvent(QCloseEvent *);

private:
    Ui::ViewUpdateSparePart *ui;

    QSqlDatabase sparePartsDB;

    QString sparePartId;

    ListSparePart *listSpareParts;
};

#endif // VIEWUPDATESPAREPART_H
