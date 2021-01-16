#ifndef STARTWINDOW_H
#define STARTWINDOW_H

#include "ListSpareParts.h"
#include "AddOrder.h"
#include "ListEmployees.h"
#include "SettingsWindow.h"
#include "ListOrders.h"
#include "ListClients.h"
#include "ListTasks.h"

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
#include <QListView>
#include <QStringListModel>
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QPointer>
#include <QSqlQuery>
#include <QCheckBox>
#include <QHBoxLayout>
#include <QDir>

class ListSparePart;
class AddOrder;
class ListOrders;
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

    ListSparePart *sparePartsTable;
    AddOrder *addOrder;
    ListEmployees *listEmployees;
    SettingsWindow *settingsWindow;
    ListOrders *listOrders;
    ListClients *listClients;
    ListTasks *listTasks;

    QSqlDatabase listTasksTable = QSqlDatabase::database("TasksTable");

    QSqlDatabase listTasksDB;
    QPointer<QSqlQueryModel> queryModel;
    QPointer<QSqlQueryModel> queryModelCheckBox;

    QTranslator translator;

    QWidget *addCheckBoxCompleted(int row_index);

protected:
    void changeEvent(QEvent *event) override;

public slots:
    void translateUI(int translate);

private slots:
    void showTime();
    void loadTasksList();
    void checkBoxStateChanged();
    void updateTasksList();

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
};

#endif // STARTWINDOW_H
