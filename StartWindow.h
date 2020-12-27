#ifndef STARTWINDOW_H
#define STARTWINDOW_H

#include "SparePartsTable.h"

#include <QDialog>
#include <QDesktopServices>
#include <QUrl>
#include <QTime>
#include <QDate>
#include <QTimer>
#include <QDebug>
#include <QElapsedTimer>

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
    void on_telegramButton_clicked();
    void on_instagramButton_clicked();
    void on_facebookButton_clicked();
    void on_viberButton_clicked();
    void on_youtubeButton_clicked();
    void on_orderFormationButton_clicked();
    void on_clientsButton_clicked();
    void on_todolistButton_clicked();
    void on_statisticsButton_clicked();
    void on_settingsButton_clicked();
};

#endif // STARTWINDOW_H
