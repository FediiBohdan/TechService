#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include "Global.h"

#include <QDebug>
#include <QDialog>
#include <QProcess>
#include <QSqlQuery>
#include <QValidator>
#include <QTranslator>
#include <QMessageBox>
#include <QApplication>
#include <QSqlDatabase>

namespace Ui {
class SettingsWindow;
}

class SettingsWindow : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsWindow(QWidget *parent = nullptr);
    ~SettingsWindow();

protected:
    void changeEvent(QEvent *event) override;

private slots:
    void openDb();
    void setLanguage();
    void setUserInfo();
    void loadSettings();
    void saveSettings();
    void saveUserData();
    void setSettingsDb();

    void on_saveSettingsButton_clicked();

private:
    Ui::SettingsWindow *ui;

    QSqlDatabase db;

    bool userError = false;

    //QTranslator translator;

signals:
    void translate(const QString &language);
    void userData(const QString &userFSName, const QString &userPosition);
};

#endif // SETTINGSWINDOW_H
