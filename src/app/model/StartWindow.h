#ifndef STARTWINDOW_H
#define STARTWINDOW_H

#include "ListTasks.h"
#include "ListOrders.h"
#include "ListClients.h"
#include "ListEmployees.h"
#include "SettingsWindow.h"
#include "ListSpareParts.h"
#include "AnalyticsDialog.h"

#include <QUrl>
#include <QTime>
#include <QDate>
#include <QDebug>
#include <QDialog>
#include <QPointer>
#include <QSqlQuery>
#include <QCheckBox>
#include <QTranslator>
#include <QSqlDatabase>
#include <QApplication>
#include <QElapsedTimer>
#include <QSqlQueryModel>
#include <QDesktopServices>

class ListOrders;
class ListClients;
class ListSparePart;
class SettingsWindow;

namespace Ui {
class StartWindow;
}

class StartWindow : public QDialog
{
    Q_OBJECT

signals:
    void closeAllWindowsExceptCurrent(bool close);

public:
    explicit StartWindow(QWidget *parent = nullptr);
    ~StartWindow();

private:
    Ui::StartWindow *ui;

    ListTasks *listTasks;
    ListOrders *listOrders;
    ListClients *listClients;
    ListEmployees *listEmployees;
    ListSparePart *sparePartsTable;
    SettingsWindow *settingsWindow;
    AnalyticsDialog *analyticsDialog;

    QSqlDatabase listTasksTable;
    QSqlDatabase listOrdersTable;
    QPointer<QSqlQueryModel> queryModel;
    QPointer<QSqlQueryModel> queryOrdersModel;
    QPointer<QSqlQueryModel> queryModelCheckBox;
    QPointer<QSqlQueryModel> queryOrdersModelCheckBox;

    QTranslator translator;

    QWidget *addCheckBoxCompleted(int rowIndex);
    QWidget *addCheckBoxOrderCompleted(int rowIndex);

protected:
    void changeEvent(QEvent *event) override;

public slots:
    void translateUI(const QString &language);
    void setUserData(const QString &userFSname, const QString &userPosition);

private slots:
    void loadTasksList();
    void loadOrdersList();
    void updateTasksList();
    void updateOrdersList();
    void loadUserSettings();
    void loadPersonalData();
    void checkBoxStateChanged();
    void checkBoxOrderStateChanged();

    void on_staffButton_clicked();
    void on_viberButton_clicked();
    void on_clientsButton_clicked();
    void on_youtubeButton_clicked();
    void on_todolistButton_clicked();
    void on_settingsButton_clicked();
    void on_telegramButton_clicked();
    void on_facebookButton_clicked();
    void on_catalogueButton_clicked();
    void on_instagramButton_clicked();    
    void on_analyticsButton_clicked();
    void on_orderFormationButton_clicked();
};

#endif // STARTWINDOW_H
