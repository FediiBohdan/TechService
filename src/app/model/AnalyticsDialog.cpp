//#include "AnalyticsDialog.h"
//#include "ui_AnalyticsDialog.h"

//// Define the scope for your variables and functions
//QT_CHARTS_USE_NAMESPACE

//AnalyticsDialog::AnalyticsDialog(QWidget *parent) :
//    QDialog(parent),
//    ui(new Ui::AnalyticsDialog)
//{
//    ui->setupUi(this);

//    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
//    setWindowFlags(windowFlags() & Qt::WindowMinimizeButtonHint);

//    employeeWorksChart();
//    mostPopularAutosChart();
//    fulfilledOrdersChart();
//    profitChart();
//    testChart();
//}

//AnalyticsDialog::~AnalyticsDialog()
//{
//    delete ui;
//}

//void AnalyticsDialog::employeeWorksChart()
//{
//    QLineSeries *lineseries = new QLineSeries();
//    lineseries->append(QPoint(0, 4));
//    lineseries->append(QPoint(1, 15));
//    lineseries->append(QPoint(2, 20));
//    lineseries->append(QPoint(3, 4));
//    lineseries->append(QPoint(4, 12));
//    lineseries->append(QPoint(5, 17));

//    QChart *chart = new QChart();
//    chart->legend()->hide();
//    chart->addSeries(lineseries);
//    chart->setAnimationOptions(QChart::AllAnimations);

//    QFont font;
//    font.setPixelSize(16);
//    chart->setTitle("1");
//    chart->setTitleFont(font);
//    chart->setTitleBrush(QBrush(Qt::black));

//    QPen pen(Qt::black);
//    pen.setWidth(3);
//    lineseries->setPen(pen);

//    QStringList categories;
//    categories << "1994" << "1995" << "1996" << "1997" << "1998" << "1999";
//    QBarCategoryAxis *axisX = new QBarCategoryAxis();
//    axisX->append(categories);
//    chart->addAxis(axisX, Qt::AlignBottom);
//    lineseries->attachAxis(axisX);
//    axisX->setRange(QString("1994"), QString("1999"));

//    QValueAxis *axisY = new QValueAxis();
//    chart->addAxis(axisY, Qt::AlignLeft);
//    lineseries->attachAxis(axisY);
//    axisY->setRange(0, 20);

//    QChartView *chartView = new QChartView(chart);
//    chartView->setRenderHint(QPainter::Antialiasing);

//    ui->verticalWidget->setMinimumHeight(451);
//    ui->verticalLayout->addWidget(chartView);
//}

//void AnalyticsDialog::mostPopularAutosChart()
//{
//    QLineSeries *lineseries = new QLineSeries();
//    lineseries->append(QPoint(0, 4));
//    lineseries->append(QPoint(1, 15));
//    lineseries->append(QPoint(2, 20));
//    lineseries->append(QPoint(3, 4));
//    lineseries->append(QPoint(4, 12));
//    lineseries->append(QPoint(5, 17));

//    QChart *chart = new QChart();
//    chart->legend()->hide();
//    chart->addSeries(lineseries);
//    chart->setAnimationOptions(QChart::AllAnimations);

//    QFont font;
//    font.setPixelSize(16);
//    chart->setTitle("2");
//    chart->setTitleFont(font);
//    chart->setTitleBrush(QBrush(Qt::black));

//    QPen pen(Qt::black);
//    pen.setWidth(3);
//    lineseries->setPen(pen);

//    QStringList categories;
//    categories << "1994" << "1995" << "1996" << "1997" << "1998" << "1999";
//    QBarCategoryAxis *axisX = new QBarCategoryAxis();
//    axisX->append(categories);
//    chart->addAxis(axisX, Qt::AlignBottom);
//    lineseries->attachAxis(axisX);
//    axisX->setRange(QString("1994"), QString("1999"));

//    QValueAxis *axisY = new QValueAxis();
//    chart->addAxis(axisY, Qt::AlignLeft);
//    lineseries->attachAxis(axisY);
//    axisY->setRange(0, 20);

//    QChartView *chartView = new QChartView(chart);
//    chartView->setRenderHint(QPainter::Antialiasing);

//    ui->verticalLayout2->addWidget(chartView);
//}

//void AnalyticsDialog::fulfilledOrdersChart()
//{
//    QLineSeries *lineseries = new QLineSeries();
//    lineseries->append(QPoint(0, 4));
//    lineseries->append(QPoint(1, 15));
//    lineseries->append(QPoint(2, 20));
//    lineseries->append(QPoint(3, 4));
//    lineseries->append(QPoint(4, 12));
//    lineseries->append(QPoint(5, 17));

//    QChart *chart = new QChart();
//    chart->legend()->hide();
//    chart->addSeries(lineseries);
//    chart->setAnimationOptions(QChart::AllAnimations);

//    QFont font;
//    font.setPixelSize(16);
//    chart->setTitle("3");
//    chart->setTitleFont(font);
//    chart->setTitleBrush(QBrush(Qt::black));

//    QPen pen(Qt::black);
//    pen.setWidth(3);
//    lineseries->setPen(pen);

//    QStringList categories;
//    categories << "1994" << "1995" << "1996" << "1997" << "1998" << "1999";
//    QBarCategoryAxis *axisX = new QBarCategoryAxis();
//    axisX->append(categories);
//    chart->addAxis(axisX, Qt::AlignBottom);
//    lineseries->attachAxis(axisX);
//    axisX->setRange(QString("1994"), QString("1999"));

//    QValueAxis *axisY = new QValueAxis();
//    chart->addAxis(axisY, Qt::AlignLeft);
//    lineseries->attachAxis(axisY);
//    axisY->setRange(0, 20);

//    QChartView *chartView = new QChartView(chart);
//    chartView->setRenderHint(QPainter::Antialiasing);

//    ui->verticalLayout3->addWidget(chartView);
//}

//void AnalyticsDialog::profitChart()
//{
//    QLineSeries *lineseries = new QLineSeries();
//    lineseries->append(QPoint(0, 4));
//    lineseries->append(QPoint(1, 15));
//    lineseries->append(QPoint(2, 20));
//    lineseries->append(QPoint(3, 4));
//    lineseries->append(QPoint(4, 12));
//    lineseries->append(QPoint(5, 17));

//    QChart *chart = new QChart();
//    chart->legend()->hide();
//    chart->addSeries(lineseries);
//    chart->setAnimationOptions(QChart::AllAnimations);

//    QFont font;
//    font.setPixelSize(16);
//    chart->setTitle("4");
//    chart->setTitleFont(font);
//    chart->setTitleBrush(QBrush(Qt::black));

//    QPen pen(Qt::black);
//    pen.setWidth(3);
//    lineseries->setPen(pen);

//    QStringList categories;
//    categories << "1994" << "1995" << "1996" << "1997" << "1998" << "1999";
//    QBarCategoryAxis *axisX = new QBarCategoryAxis();
//    axisX->append(categories);
//    chart->addAxis(axisX, Qt::AlignBottom);
//    lineseries->attachAxis(axisX);
//    axisX->setRange(QString("1994"), QString("1999"));

//    QValueAxis *axisY = new QValueAxis();
//    chart->addAxis(axisY, Qt::AlignLeft);
//    lineseries->attachAxis(axisY);
//    axisY->setRange(0, 20);

//    QChartView *chartView = new QChartView(chart);
//    chartView->setRenderHint(QPainter::Antialiasing);

//    ui->verticalLayout4->addWidget(chartView);
//}

//void AnalyticsDialog::testChart()
//{
//    QLineSeries *lineseries = new QLineSeries();
//    lineseries->append(QPoint(0, 4));
//    lineseries->append(QPoint(1, 15));
//    lineseries->append(QPoint(2, 20));
//    lineseries->append(QPoint(3, 4));
//    lineseries->append(QPoint(4, 12));
//    lineseries->append(QPoint(5, 17));

//    QChart *chart = new QChart();
//    chart->legend()->hide();
//    chart->addSeries(lineseries);
//    chart->setAnimationOptions(QChart::AllAnimations);

//    QFont font;
//    font.setPixelSize(16);
//    chart->setTitle("5");
//    chart->setTitleFont(font);
//    chart->setTitleBrush(QBrush(Qt::black));

//    QPen pen(Qt::black);
//    pen.setWidth(3);
//    lineseries->setPen(pen);

//    QStringList categories;
//    categories << "1994" << "1995" << "1996" << "1997" << "1998" << "1999";
//    QBarCategoryAxis *axisX = new QBarCategoryAxis();
//    axisX->append(categories);
//    chart->addAxis(axisX, Qt::AlignBottom);
//    lineseries->attachAxis(axisX);
//    axisX->setRange(QString("1994"), QString("1999"));

//    QValueAxis *axisY = new QValueAxis();
//    chart->addAxis(axisY, Qt::AlignLeft);
//    lineseries->attachAxis(axisY);
//    axisY->setRange(0, 20);

//    QChartView *chartView = new QChartView(chart);
//    chartView->setRenderHint(QPainter::Antialiasing);

//    ui->verticalLayout5->addWidget(chartView);
//}

