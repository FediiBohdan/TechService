#ifndef ORDERCREATION_H
#define ORDERCREATION_H

#include "StaffTable.h"
#include "SparePartsTable.h"

#include <QDialog>
#include <QQmlApplicationEngine>
#include <QDebug>
#include <QDate>
#include <QTime>

class StartWindow;
class SparePartsTable;

namespace Ui {
class OrderCreation;
}

class OrderCreation : public QDialog
{
    Q_OBJECT

public:
    explicit OrderCreation(QWidget *parent = nullptr);
    ~OrderCreation();

private:
    Ui::OrderCreation *ui;

    StaffTable *taffTable;
    SparePartsTable *sparePartsTable;

private slots:
    void openMap();
    void createOrder();

public slots:
    void closeWindow();
};

#endif // ORDERCREATION_H
