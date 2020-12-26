#ifndef STARTWINDOW_H
#define STARTWINDOW_H

#include <QDialog>

#include "SparePartsTable.h"

namespace Ui {
class StartWindow;
}

class StartWindow : public QDialog
{
    Q_OBJECT

public:
    explicit StartWindow(QWidget *parent = nullptr);
    ~StartWindow();

private:
    Ui::StartWindow *ui;

    SparePartsTable* sparePartsTable;

private slots:
    void showTime();
    void on_catalogueButton_clicked();
};

#endif // STARTWINDOW_H
