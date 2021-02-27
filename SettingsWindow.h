#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include "Global.h"
#include "StartWindow.h"

#include <QDialog>
#include <QTranslator>
#include <QApplication>
#include <QMessageBox>
#include <QProcess>
#include <QDebug>

class StartWindow;

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
    void setLanguage();
    void loadSettings();
    void saveSettings();
    void setUserInfo();
    void saveUserData();

    void on_saveSettingsButton_clicked();

private:
    Ui::SettingsWindow *ui;

    QTranslator translator;

signals:
    void translate(int translate);
    void userData(const QString &userFSName, const QString &userPosition);
};

#endif // SETTINGSWINDOW_H
