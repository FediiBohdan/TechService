#ifndef ListSpareParts_H
#define ListSpareParts_H

#include "StartWindow.h"
#include "AddSparePart.h"
#include "ViewUpdateSparePart.h"

#include <QFile>
#include <QLabel>
#include <QDebug>
#include <QDialog>
#include <QPointer>
#include <QTextEdit>
#include <QTextStream>
#include <QFileDialog>
#include <QMessageBox>
#include <QSqlQueryModel>

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

    QSqlDatabase notificationTable;
    QPointer<QSqlQueryModel> queryModel;

    QWidget *widget;
    QTextEdit *textEdit;
    QPushButton *button;
    StartWindow *startWindow;
    AddSparePart *addSparePart;
    ViewUpdateSparePart *viewUpdateSparePart;

    QString autoModel;

    bool searchFlag;
    bool autoSearchFlag;

private slots:
    void loadTable();
    void saveAsCSV(QString fileName);
    void createNotification();
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
    void on_notificationCreation_clicked();

public slots:
    void closeWindow();
};

#endif // ListSpareParts_H
