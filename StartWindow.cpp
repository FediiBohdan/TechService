#include "StartWindow.h"
#include "ui_StartWindow.h"

#include <QTime>
#include <QDate>
#include <QTimer>
#include <QDebug>
#include <QElapsedTimer>

QElapsedTimer elapsedTimer;
QTimer* timer = new QTimer();
auto countdown = QTime(8, 0, 0);

StartWindow::StartWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StartWindow)
{
    ui->setupUi(this);

    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowFlags(windowFlags() & Qt::WindowMinimizeButtonHint);

    StartWindow::showNormal();
    StartWindow::showMaximized();

    elapsedTimer.start();
    connect(timer, SIGNAL(timeout()), this, SLOT(showTime()));
    timer->start(1000);
}

StartWindow::~StartWindow()
{
    delete ui;
}

void StartWindow::showTime(){
    auto elapsed = elapsedTimer.elapsed();
    auto c = countdown.addMSecs(-elapsed);
    QString timestr = c.toString("hh:mm:ss");
    ui->lcdNumber->display(timestr);
}
