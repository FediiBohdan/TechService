#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include "Global.h"

#include <QDialog>
#include <QTranslator>
#include <QApplication>
#include <QMessageBox>
#include <QProcess>

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
    // Метод получения событий в главном окне приложения. В нём будет производиться проверка события смены перевода приложения
    //void changeEvent(QEvent * event) override;

private slots:
    void setLanguage();
    void loadSettings();
    void saveSettings();
    void on_saveSettingsButton_clicked();

private:
    Ui::SettingsWindow *ui;
};

#endif // SETTINGSWINDOW_H
