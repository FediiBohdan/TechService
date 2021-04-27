#include "AnalyticsDialog.h"
#include "ui_AnalyticsDialog.h"

AnalyticsDialog::AnalyticsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AnalyticsDialog)
{
    ui->setupUi(this);

    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowFlags(windowFlags() & Qt::WindowMinimizeButtonHint);

    connect(ui->showAnalyticsButton, &QAbstractButton::clicked, this, &AnalyticsDialog::profitAnalytics);
    connect(ui->orderShowAnalyticsButton, &QAbstractButton::clicked, this, &AnalyticsDialog::fulfilledOrdersAnalytics);

    QStringList services = {tr("Все"), "Среднефонтанская, 30А (Приморский р-н)", "Платонова, 56 (Малиновский р-н)", "Архитекторская, 28 (Киевский р-н)"};
    QStringList months = {tr("Январь"), tr("Февраль"), tr("Март"), tr("Апрель"), tr("Май"), tr("Июнь"), tr("Июль"), tr("Август"), tr("Сентябрь"), tr("Октябрь"),
                          tr("Ноябрь"), tr("Декабрь")};

    ui->servicesComboBox->addItems(services);
    ui->monthsComboBox->addItems(months);
    ui->orderServicesComboBox->addItems(services);
    ui->orderMonthsComboBox->addItems(months);

    mostPopularAutosAnalytics();
    mostPopularSparePartsAnalytics();
}

AnalyticsDialog::~AnalyticsDialog()
{
    delete ui;
}

/**
 * Calculates most popular autos in car services.
 */
void AnalyticsDialog::mostPopularAutosAnalytics()
{
    queryMostPopularAutosAnalytics = new QSqlQueryModel(this);

    QString queryString = "SELECT auto_brand, COUNT(*) FROM orders_history GROUP BY auto_brand HAVING COUNT(*) > 1 ORDER BY COUNT(*) DESC";

    queryMostPopularAutosAnalytics->setQuery(queryString);

    queryMostPopularAutosAnalytics->setHeaderData(0, Qt::Horizontal, tr("Марка авто"));
    queryMostPopularAutosAnalytics->setHeaderData(1, Qt::Horizontal, tr("Количество"));

    ui->mostPopularAutosTableView->setModel(queryMostPopularAutosAnalytics);

    ui->mostPopularAutosTableView->verticalHeader()->setVisible(false);
    ui->mostPopularAutosTableView->horizontalHeader()->setDefaultSectionSize(maximumWidth());
    ui->mostPopularAutosTableView->resizeColumnsToContents();
    ui->mostPopularAutosTableView->resizeRowsToContents();
}

/**
 * Calculates most used spare parts in car services.
 */
void AnalyticsDialog::mostPopularSparePartsAnalytics()
{
    querySparePartsAnalytics = new QSqlQueryModel(this);

    QString queryString = "SELECT order_spare_part, COUNT(*) FROM order_spare_parts GROUP BY order_spare_part HAVING COUNT(*) > 1 ORDER BY COUNT(*) DESC";

    querySparePartsAnalytics->setQuery(queryString);

    querySparePartsAnalytics->setHeaderData(0, Qt::Horizontal, tr("Название"));
    querySparePartsAnalytics->setHeaderData(1, Qt::Horizontal, tr("Количество"));

    ui->sparePartsTableView->setModel(querySparePartsAnalytics);

    ui->sparePartsTableView->verticalHeader()->setVisible(false);
    ui->sparePartsTableView->horizontalHeader()->setDefaultSectionSize(maximumWidth());
    ui->sparePartsTableView->resizeColumnsToContents();
    ui->sparePartsTableView->resizeRowsToContents();
}

/**
 * Calculates number of fulfilled orders in car services.
 */
void AnalyticsDialog::fulfilledOrdersAnalytics()
{
    QSqlQuery queryFulfilledOrdersAnalytics(analyticsTable);

    QString year = ui->ordersYearLine->text();

    QString month = "0";
    if (ui->orderMonthsComboBox->currentIndex() == 0)
        month = "01";
    else if (ui->orderMonthsComboBox->currentIndex() == 1)
        month = "02";
    else if (ui->orderMonthsComboBox->currentIndex() == 2)
        month = "03";
    else if (ui->orderMonthsComboBox->currentIndex() == 3)
        month = "04";
    else if (ui->orderMonthsComboBox->currentIndex() == 4)
        month = "05";
    else if (ui->orderMonthsComboBox->currentIndex() == 5)
        month = "06";
    else if (ui->orderMonthsComboBox->currentIndex() == 6)
        month = "07";
    else if (ui->orderMonthsComboBox->currentIndex() == 7)
        month = "08";
    else if (ui->orderMonthsComboBox->currentIndex() == 8)
        month = "09";
    else if (ui->orderMonthsComboBox->currentIndex() == 9)
        month = "10";
    else if (ui->orderMonthsComboBox->currentIndex() == 10)
        month = "11";
    else if (ui->orderMonthsComboBox->currentIndex() == 11)
        month = "12";

    QString query = "SELECT creation_date, COUNT(is_ready) FROM orders_history WHERE is_ready = 1 AND creation_date LIKE'%" + (month + "." + year) + "%'";
    QString queryService;

    if ((ui->orderServicesComboBox->currentIndex() == 1) || (ui->orderServicesComboBox->currentIndex() == 2) || (ui->orderServicesComboBox->currentIndex() == 3))
        queryService.append(" AND service_address = '" + ui->orderServicesComboBox->currentText() + "'");

    query.append(queryService);
    queryFulfilledOrdersAnalytics.prepare(query);
    queryFulfilledOrdersAnalytics.exec();

    if (queryFulfilledOrdersAnalytics.first() == true)
    {
        QString fulfilledOrderAmount = queryFulfilledOrdersAnalytics.value(1).toString();

        ui->ordersAmountLine->setText(fulfilledOrderAmount);
    }
    else
        ui->ordersAmountLine->setText("0");
}

/**
 * Calculates profit by car services.
 */
void AnalyticsDialog::profitAnalytics()
{
    QSqlQuery queryProfitAnalytics(analyticsTable);

    QString year = ui->yearLine->text();

    QString month = "0";
    if (ui->monthsComboBox->currentIndex() == 0)
        month = "01";
    else if (ui->monthsComboBox->currentIndex() == 1)
        month = "02";
    else if (ui->monthsComboBox->currentIndex() == 2)
        month = "03";
    else if (ui->monthsComboBox->currentIndex() == 3)
        month = "04";
    else if (ui->monthsComboBox->currentIndex() == 4)
        month = "05";
    else if (ui->monthsComboBox->currentIndex() == 5)
        month = "06";
    else if (ui->monthsComboBox->currentIndex() == 6)
        month = "07";
    else if (ui->monthsComboBox->currentIndex() == 7)
        month = "08";
    else if (ui->monthsComboBox->currentIndex() == 8)
        month = "09";
    else if (ui->monthsComboBox->currentIndex() == 9)
        month = "10";
    else if (ui->monthsComboBox->currentIndex() == 10)
        month = "11";
    else if (ui->monthsComboBox->currentIndex() == 11)
        month = "12";

    QString query = "SELECT creation_date, SUM(price) FROM orders_history WHERE creation_date LIKE'%" + (month + "." + year) + "%'";
    QString queryService;

    if ((ui->servicesComboBox->currentIndex() == 1) || (ui->servicesComboBox->currentIndex() == 2) || (ui->servicesComboBox->currentIndex() == 3))
        queryService.append(" AND service_address = '" + ui->servicesComboBox->currentText() + "'");

    query.append(queryService);
    queryProfitAnalytics.prepare(query);
    queryProfitAnalytics.exec();

    if (queryProfitAnalytics.first() == true)
    {
        float fullProfit = queryProfitAnalytics.value(1).toFloat();
        float netProfit = (fullProfit * 15) / 100;
        QString netProfitFinal = QString("%1").arg(netProfit, 0, 'f', 2);

        ui->profitLine->setText(netProfitFinal);
    }
    else
        ui->profitLine->setText("0.00");
}
