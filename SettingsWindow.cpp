#include "SettingsWindow.h"
#include "ui_SettingsWindow.h"

SettingsWindow::SettingsWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsWindow)
{
    ui->setupUi(this);

    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowFlags(windowFlags() & Qt::WindowMinimizeButtonHint);

    ui->languageSelection->addItems(QStringList() << "Русский" << "Українська" << "English");
    connect(ui->languageSelection, &QComboBox::currentTextChanged, this, &SettingsWindow::setLanguage);

    QString registerLanguage = global::getSettingsValue("language", "settings").toString();

    if (registerLanguage.isEmpty())
       ui->languageSelection->setCurrentIndex(0);

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
    QTranslator translator;

    if (!translator.isEmpty())
        qApp->removeTranslator(&translator);

    if (ui->languageSelection->currentIndex() == 0)
    {
       translator.load(":/translations/russian.qm");
       emit translate("russian"); //pass translation parameter for simultaneous dynamic GUI language changes
    }
    else if (ui->languageSelection->currentIndex() == 1)
    {
       translator.load(":/translations/ukrainian.qm");
       emit translate("ukrainian");
    }
    else if (ui->languageSelection->currentIndex() == 2)
    {
       translator.load(":/translations/english.qm");
       emit translate("english");
    }

    qApp->installTranslator(&translator);
    saveSettings();
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

    QDialog::close();

    QMessageBox::information(this, tr("Уведомление"), tr("Настройки успешно сохранены!"), QMessageBox::Ok);
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

    emit userData(userFSname, userPosition);
}
