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
    void changeEvent(QEvent * event) override;

private slots:
    void setLanguage();
    void loadSettings();
    void saveSettings();

private:
    Ui::SettingsWindow *ui;

    QTranslator translator;

    StartWindow *startWindow;

signals:
    void translate(int translate);
};

#endif // SETTINGSWINDOW_H
