#ifndef ANALYTICSDIALOG_H
#define ANALYTICSDIALOG_H

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
};

#endif // ANALYTICSDIALOG_H
