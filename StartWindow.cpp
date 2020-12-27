#include "StartWindow.h"
#include "ui_StartWindow.h"

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

void StartWindow::on_orderFormationButton_clicked()
{

}

void StartWindow::on_clientsButton_clicked()
{

}

void StartWindow::on_todolistButton_clicked()
{

}

void StartWindow::on_catalogueButton_clicked()
{
    sparePartsTable = new SparePartsTable;
    sparePartsTable->show();
    sparePartsTable->setAttribute(Qt::WA_DeleteOnClose);
}

void StartWindow::on_statisticsButton_clicked()
{

}

void StartWindow::on_settingsButton_clicked()
{

}

void StartWindow::on_telegramButton_clicked()
{
    QString telegramLink = "https://web.telegram.org/";
    QDesktopServices::openUrl(QUrl(telegramLink));
}

void StartWindow::on_instagramButton_clicked()
{
    QString instagramLink = "https://www.instagram.com/";
    QDesktopServices::openUrl(QUrl(instagramLink));
}

void StartWindow::on_facebookButton_clicked()
{
    QString facebookLink = "https://www.facebook.com/";
    QDesktopServices::openUrl(QUrl(facebookLink));
}

void StartWindow::on_viberButton_clicked()
{
    QString viberLink = "https://www.viber.com/";
    QDesktopServices::openUrl(QUrl(viberLink));
}

void StartWindow::on_youtubeButton_clicked()
{
    QString youtubeLink = "https://www.youtube.com/";
    QDesktopServices::openUrl(QUrl(youtubeLink));
}
