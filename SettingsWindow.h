#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include "Global.h"

#include <QDebug>
#include <QDialog>
#include <QProcess>
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

    void setDatabases(const QSqlDatabase &db);

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

    QSqlDatabase m_db;

    //QTranslator translator;

signals:
    void translate(const QString &language);
    void userData(const QString &userFSName, const QString &userPosition);
};

#endif // SETTINGSWINDOW_H
