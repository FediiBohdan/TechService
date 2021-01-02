#include "SettingsWindow.h"
#include "ui_SettingsWindow.h"

SettingsWindow::SettingsWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsWindow)
{
    ui->setupUi(this);

    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowFlags(windowFlags() & Qt::WindowMinimizeButtonHint);

    setLanguage();
    loadSettings();
}

SettingsWindow::~SettingsWindow()
{
    delete ui;
}

//void SettingsWindow::changeEvent(QEvent *event)
//{
//    // В случае получения события изменения языка приложения
//    if (event->type() == QEvent::LanguageChange) {
//        ui->retranslateUi(this);    // переведём окно заново
//    }
//}

void SettingsWindow::setLanguage()
{
    ui->languageSelection->addItems(QStringList() << "Русский" << "Українська" << "English");

    QString registerLanguage = global::getSettingsValue("language", "settings").toString();

       if (registerLanguage == "")
           ui->languageSelection->setCurrentIndex(0);
       else
           ui->languageSelection->setCurrentIndex(ui->languageSelection->findData(registerLanguage, Qt::UserRole, Qt::MatchExactly));
}

void SettingsWindow::loadSettings()
{
    ui->languageSelection->setCurrentText(global::getSettingsValue("language", "settings").toString());
}

void SettingsWindow::saveSettings()
{
    global::setSettingsValue("language", ui->languageSelection->currentText(), "settings");
}

void SettingsWindow::on_saveSettingsButton_clicked()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle(tr("Применение настроек"));
    msgBox.setText(tr("Для применения настроек необходимо перезагрузить приложение. Перезагрузить сейчас?"));
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setButtonText(QMessageBox::Yes, tr("Да"));
    msgBox.setButtonText(QMessageBox::No, tr("Нет"));
    qint32 reply = msgBox.exec();

    switch (reply)
    {
    case QMessageBox::Yes:
        saveSettings();

        qApp->closeAllWindows();
        qApp->quit();

        QProcess::startDetached(qApp->arguments()[0], QStringList() << "restart");
        break;
    case QMessageBox::No:
        msgBox.close();
        break;
    default:
        break;
    }
}
