#include "AnalyticsDialog.h"
#include "ui_AnalyticsDialog.h"

// Widget used to display charts
#include <QtCharts/QChartView>

// Used to draw bars representing data provided
// grouped into categories
#include <QtCharts/QBarSeries>

// Represents 1 set of bars in a bar chart
#include <QtCharts/QBarSet>

// Displays the color used to represent each
// QBarSet
#include <QtCharts/QLegend>

// Adds categories to the charts axes
#include <QtCharts/QBarCategoryAxis>

// Used to create stacked bar charts
#include <QtCharts/QHorizontalStackedBarSeries>

// Used to create a line chart
#include <QtCharts/QLineSeries>

// Used to change names on axis
#include <QtCharts/QCategoryAxis>

// Define the scope for your variables and functions
QT_CHARTS_USE_NAMESPACE

AnalyticsDialog::AnalyticsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AnalyticsDialog)
{
    ui->setupUi(this);

    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowFlags(windowFlags() & Qt::WindowMinimizeButtonHint);

    //QDialog::showMaximized();

    QDir tempDirDB = QDir::currentPath(); tempDirDB.cdUp(); QString dirDB = tempDirDB.path();

    analiticsDB = QSqlDatabase::addDatabase("QSQLITE");
    analiticsDB.setDatabaseName(dirDB + "\\CRM_AutoService\\ServiceStationDB.db");
    analiticsDB.open();

    employeeWorksChart();
}

AnalyticsDialog::~AnalyticsDialog()
{
    delete ui;
}

void AnalyticsDialog::employeeWorksChart()
{
    QLineSeries *lineseries = new QLineSeries();
    lineseries->append(QPoint(0, 4));
    lineseries->append(QPoint(1, 15));
    lineseries->append(QPoint(2, 20));
    lineseries->append(QPoint(3, 4));
    lineseries->append(QPoint(4, 12));
    lineseries->append(QPoint(5, 17));

    QChart *chart = new QChart();
    chart->legend()->hide();
    chart->addSeries(lineseries);
    chart->setAnimationOptions(QChart::AllAnimations);

    QFont font;
    font.setPixelSize(16);
    chart->setTitle("Title");
    chart->setTitleFont(font);
    chart->setTitleBrush(QBrush(Qt::black));

    QPen pen(Qt::black);
    pen.setWidth(3);
    lineseries->setPen(pen);

    QStringList categories;
    categories << "1994" << "1995" << "1996" << "1997" << "1998" << "1999";
    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    chart->addAxis(axisX, Qt::AlignBottom);
    lineseries->attachAxis(axisX);
    axisX->setRange(QString("1994"), QString("1999"));

    QValueAxis *axisY = new QValueAxis();
    chart->addAxis(axisY, Qt::AlignLeft);
    lineseries->attachAxis(axisY);
    axisY->setRange(0, 20);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    //chartView->setMinimumSize(946, 516);
    chartView->setParent(ui->horizontalWidget);
}
