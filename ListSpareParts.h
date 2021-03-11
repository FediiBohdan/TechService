#ifndef ListSpareParts_H
#define ListSpareParts_H

#include "StartWindow.h"
#include "AddSparePart.h"
#include "ViewUpdateSparePart.h"

#include <QDialog>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QPointer>
#include <QSqlQuery>
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QFileDialog>

class StartWindow;
class AddSparePart;
class ViewUpdateSparePart;

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
    ViewUpdateSparePart *viewUpdateSparePart;

    QString autoModel;

    bool searchFlag;
    bool autoSearchFlag;

private slots:
    void loadTable();
    void saveAsCSV(QString filename);
    void showSparePartInfo(const QModelIndex &index);

    void on_addSparePartButton_clicked();
    void on_sparePartSearch_returnPressed();
    void on_updateButton_clicked();
    void on_toyotaSearchButton_clicked();
    void on_mitsubishiSearchButton_clicked();
    void on_nissanSearchButton_clicked();
    void on_mazdaSearchButton_clicked();
    void on_hondaSearchButton_clicked();
    void on_lexusSearchButton_clicked();
    void on_subaruSearchButton_clicked();
    void on_suzukiSearchButton_clicked();
    void on_infinitiSearchButton_clicked();
    void on_isuzuSearchButton_clicked();
    void on_acuraSearchButton_clicked();
    void on_csvExportButton_clicked();

public slots:
    void closeWindow();
};

#endif // ListSpareParts_H
