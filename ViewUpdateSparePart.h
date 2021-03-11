#ifndef VIEWUPDATESPAREPART_H
#define VIEWUPDATESPAREPART_H

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

private:
    Ui::ViewUpdateSparePart *ui;

    QSqlDatabase sparePartsDB;

    QString sparePartId;
};

#endif // VIEWUPDATESPAREPART_H
