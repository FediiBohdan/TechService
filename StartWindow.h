#ifndef STARTWINDOW_H
#define STARTWINDOW_H

#include "AddOrder.h"
#include "ListTasks.h"
#include "ListOrders.h"
#include "ListClients.h"
#include "ListEmployees.h"
#include "SettingsWindow.h"
#include "ListSpareParts.h"

#include <QUrl>
#include <QTime>
#include <QDate>
#include <QTimer>
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

class ListSparePart;
class AddOrder;
class ListOrders;
class ListClients;

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

    AddOrder *addOrder;
    ListTasks *listTasks;
    ListOrders *listOrders;
    ListClients *listClients;
    ListEmployees *listEmployees;
    SettingsWindow *settingsWindow;
    ListSparePart *sparePartsTable;

    QSqlDatabase listTasksTable;
    QPointer<QSqlQueryModel> queryModel;
    QPointer<QSqlQueryModel> queryModelCheckBox;

    QTranslator translator;

    QWidget *addCheckBoxCompleted(int rowIndex);

protected:
    void changeEvent(QEvent *event) override;

public slots:
    void translateUI(const QString &language);
    void setUserData(const QString &userFSname, const QString &userPosition);

private slots:
    void showTime();
    void loadTasksList();    
    void updateTasksList();
    void loadUserSettings();
    void checkBoxStateChanged();

    void on_staffButton_clicked();
    void on_viberButton_clicked();
    void on_clientsButton_clicked();
    void on_desktopButton_clicked();
    void on_youtubeButton_clicked();
    void on_todolistButton_clicked();
    void on_settingsButton_clicked();
    void on_telegramButton_clicked();
    void on_facebookButton_clicked();
    void on_catalogueButton_clicked();
    void on_instagramButton_clicked();
    void on_orderFormationButton_clicked();
};

#endif // STARTWINDOW_H
