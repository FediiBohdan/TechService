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

void SettingsWindow::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::LanguageChange)
        ui->retranslateUi(this);
}

void SettingsWindow::setLanguage()
{
    startWindow = new StartWindow;
    startWindow->close();

    ui->languageSelection->addItems(QStringList() << "Русский" << "Українська" << "English");

    connect(ui->languageSelection, static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentTextChanged), [=]
    {
        if (ui->languageSelection->currentIndex() == 0)
        {
           translator.load(":/translations/russian.qm");
           qApp->installTranslator(&translator);
           saveSettings();

           // pass translation parameter to classes for simultaneous dynamic translation changes
           connect(this, &SettingsWindow::translate, startWindow, &StartWindow::translateUI);
           emit translate(0);
        }
        else if (ui->languageSelection->currentIndex() == 1)
        {
           translator.load(":/translations/ukrainian.qm");
           qApp->installTranslator(&translator);
           saveSettings();

           connect(this, &SettingsWindow::translate, startWindow, &StartWindow::translateUI);
           emit translate(1);
        }
        else if (ui->languageSelection->currentIndex() == 2)
        {
           translator.load(":/translations/english.qm");
           qApp->installTranslator(&translator);
           saveSettings();

           connect(this, &SettingsWindow::translate, startWindow, &StartWindow::translateUI);
           emit translate(2);
        }
    });

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
