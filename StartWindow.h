#ifndef STARTWINDOW_H
#define STARTWINDOW_H

#include "ListSpareParts.h"
#include "AddOrder.h"
#include "ListEmployees.h"
#include "SettingsWindow.h"
#include "ListOrders.h"
#include "ListClients.h"

#include <QDialog>
#include <QDesktopServices>
#include <QUrl>
#include <QTime>
#include <QDate>
#include <QTimer>
#include <QDebug>
#include <QElapsedTimer>
#include <QApplication>
#include <QTranslator>

class ListSparePart;
class AddOrder;
class ListOrders;

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

    ListSparePart *sparePartsTable;
    AddOrder *addOrder;
    ListEmployees *listEmployees;
    SettingsWindow *settingsWindow;
    ListOrders *listOrders;
    ListClients *listClients;

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
    void on_desktopButton_clicked();
    void on_staffButton_clicked();
    void on_todolistButton_clicked();
    void on_statisticsButton_clicked();
    void on_settingsButton_clicked();

signals:
    void closeAllWindowsExceptCurrent(bool close);
};

#endif // STARTWINDOW_H
