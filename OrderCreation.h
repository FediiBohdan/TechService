#ifndef ORDERCREATION_H
#define ORDERCREATION_H

#include <QDialog>
#include <QQmlApplicationEngine>

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

private slots:
    void openMap();
};

#endif // ORDERCREATION_H
