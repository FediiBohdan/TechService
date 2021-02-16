#ifndef ANALYTICSDIALOG_H
#define ANALYTICSDIALOG_H

#include <QDialog>

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
};

#endif // ANALYTICSDIALOG_H
