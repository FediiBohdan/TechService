#ifndef ANALYTICSDIALOG_H
#define ANALYTICSDIALOG_H

#include <QDate>
#include <QDebug>
#include <QDialog>
#include <QPointer>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QSqlQueryModel>

namespace Ui {
class AnalyticsDialog;
}

class AnalyticsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AnalyticsDialog(QWidget *parent = nullptr);
    ~AnalyticsDialog();

private:
    Ui::AnalyticsDialog *ui;

    QSqlDatabase analyticsTable;

    QPointer<QSqlQueryModel> querySparePartsAnalytics;
    QPointer<QSqlQueryModel> queryMostPopularAutosAnalytics;

private slots:
    void profitAnalytics();    
    void fulfilledOrdersAnalytics();
    void mostPopularAutosAnalytics();
    void mostPopularSparePartsAnalytics();    
};

#endif // ANALYTICSDIALOG_H
