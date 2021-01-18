#ifndef UPDATESPAREPART_H
#define UPDATESPAREPART_H

#include "ViewSparePart.h"
#include "ListSpareParts.h"

#include <QDialog>
#include <QSqlDatabase>

class ViewSparePart;
class ListSparePart;

namespace Ui {
class UpdateSparePart;
}

class UpdateSparePart : public QDialog
{
    Q_OBJECT

signals:
    void sendData(bool update);

public:
    explicit UpdateSparePart(QWidget *parent = nullptr);
    ~UpdateSparePart();

public slots:
    void setValues(const QString& id);

private slots:
    void on_backToViewInfoButton_clicked();
    void on_saveUpdatedInfo_clicked();

    void closeEvent(QCloseEvent*);

private:
    Ui::UpdateSparePart *ui;

    QSqlDatabase sparePartsDB;

    QString sparePartId;

    ListSparePart *listSpareParts;
};

#endif // UPDATESPAREPART_H
