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
    setUserInfo();
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

void SettingsWindow::setUserInfo()
{
    QString registerUserFirstName = global::getSettingsValue("userFirstName", "settings").toString();
    QString registerUserSecondName = global::getSettingsValue("userSecondName", "settings").toString();
    QString registerUserThirdName = global::getSettingsValue("userThirdName", "settings").toString();
    QString registerUserPosition = global::getSettingsValue("userPosition", "settings").toString();

    if (!registerUserFirstName.isEmpty())
        ui->userFirstName->setText(registerUserFirstName);

    if (!registerUserSecondName.isEmpty())
        ui->userSecondName->setText(registerUserSecondName);

    if (!registerUserThirdName.isEmpty())
        ui->userThirdName->setText(registerUserThirdName);

    if (!registerUserPosition.isEmpty())
        ui->userPosition->setText(registerUserPosition);
}

void SettingsWindow::setLanguage()
{
    ui->languageSelection->addItems(QStringList() << "Русский" << "Українська" << "English");

    connect(ui->languageSelection, static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentTextChanged), [=] ()
    {
        if (ui->languageSelection->currentIndex() == 0)
        {
           translator.load(":/translations/russian.qm");
           qApp->installTranslator(&translator);
           saveSettings();

           // pass translation parameter to classes for simultaneous dynamic translation changes
           emit translate(0);
        }
        else if (ui->languageSelection->currentIndex() == 1)
        {
           translator.load(":/translations/ukrainian.qm");
           qApp->installTranslator(&translator);
           saveSettings();

           emit translate(1);
        }
        else if (ui->languageSelection->currentIndex() == 2)
        {
           translator.load(":/translations/english.qm");
           qApp->installTranslator(&translator);
           saveSettings();

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

void SettingsWindow::on_saveSettingsButton_clicked()
{
    saveUserData();

    //QDialog::close();
}

void SettingsWindow::saveUserData()
{
    QString userFirstName = ui->userFirstName->text();
    QString userSecondName = ui->userSecondName->text();
    QString userPosition = ui->userPosition->text();

    global::setSettingsValue("userFirstName", userFirstName, "settings");
    global::setSettingsValue("userSecondName", userSecondName, "settings");
    global::setSettingsValue("userThirdName", ui->userThirdName->text(), "settings");
    global::setSettingsValue("userPosition", userPosition, "settings");

    QString userFSname = userFirstName.append(" " + userSecondName);

    emit userData(userFSname, userPosition); qDebug() << __LINE__;
}
