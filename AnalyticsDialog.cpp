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
    QLineSeries *series = new QLineSeries(); // y
    series->append(0, 16);
    series->append(1, 25);
    series->append(2, 24);
    series->append(3, 19);
    series->append(4, 33);
    series->append(5, 25);
    series->append(6, 34);

    // Create chart, add data, hide legend, and add axis
    QChart *chart = new QChart();
    chart->legend()->hide();
    chart->addSeries(series);
    chart->createDefaultAxes();

    // Customize the title font
    QFont font;
    font.setPixelSize(16);
    chart->setTitleFont(font);
    chart->setTitleBrush(QBrush(Qt::black));
    chart->setTitle("Title");

    // Change the line color and weight
    QPen pen(QRgb(0x000000));
    pen.setWidth(3);
    series->setPen(pen);

    //NoAnimation, GridAxisAnimations, SeriesAnimations
    chart->setAnimationOptions(QChart::AllAnimations);

    // Change the x axis categories
    QCategoryAxis *axisX = new QCategoryAxis(); //x
    axisX->append("1986", 0);
    axisX->append("1987", 1);
    axisX->append("1988", 2);
    axisX->append("1989", 3);
    axisX->append("1990", 4);
    axisX->append("1991", 5);
    axisX->append("1992", 6);
    chart->setAxisX(axisX, series);
    //chart->addAxis(axisX, Qt::AlignBottom);
    //chart->addAxis(series, Qt::AlignLeft);

    // Used to display the chart
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    chartView->setMinimumSize(946, 516);


    chartView->setParent(ui->horizontalFrame);
}
