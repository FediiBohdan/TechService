#ifndef ANALYTICSDIALOG_H
#define ANALYTICSDIALOG_H

// Widget used to display charts
#include <QtCharts/QChartView>
// Adds categories to the charts axes
#include <QtCharts/QBarCategoryAxis>
// Used to create a line chart
#include <QtCharts/QLineSeries>
// Used to change names on axis
#include <QtCharts/QCategoryAxis>


#include <QDialog>
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QDir>

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


    QSqlDatabase analiticsDB;

private slots:
    void employeeWorksChart();
    void profitChart();
    void fulfilledOrdersChart();
    void mostPopularAutosChart();
    void testChart();
};

#endif // ANALYTICSDIALOG_H
