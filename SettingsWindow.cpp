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

//sets user settings to window fields
void SettingsWindow::setUserInfo()
{
    QString registerUserFirstName = global::getSettingsValue("userFirstName", "settings").toString();
    QString registerUserSecondName = global::getSettingsValue("userSecondName", "settings").toString();
    QString registerUserThirdName = global::getSettingsValue("userThirdName", "settings").toString();
    QString registerUserPosition = global::getSettingsValue("userPosition", "settings").toString();
    QString registerUserLogin = global::getSettingsValue("userLogin", "settings").toString();
    QByteArray byteUserPassword = global::getSettingsValue("userPassword", "settings").toByteArray();
    QString userPassword = QString(QByteArray::fromBase64(byteUserPassword));

    if (!registerUserFirstName.isEmpty())
        ui->userFirstName->setText(registerUserFirstName);

    if (!registerUserSecondName.isEmpty())
        ui->userSecondName->setText(registerUserSecondName);

    if (!registerUserThirdName.isEmpty())
        ui->userThirdName->setText(registerUserThirdName);

    if (!registerUserPosition.isEmpty())
        ui->userPosition->setText(registerUserPosition);

    if (!registerUserLogin.isEmpty())
        ui->userLogin->setText(registerUserLogin);

    if (!userPassword.isEmpty())
        ui->userPassword->setText(userPassword);
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

//sets DB settings to window fields
void SettingsWindow::loadSettings()
{
    ui->languageSelection->setCurrentText(global::getSettingsValue("language", "settings").toString());

    QByteArray bytePassword = global::getSettingsValue("passwordDB", "settings").toByteArray();
    QString password = QString(QByteArray::fromBase64(bytePassword));
    ui->hostName->setText(global::getSettingsValue("hostName", "settings").toString());
    ui->dbName->setText(global::getSettingsValue("databaseName", "settings").toString());
    ui->userName->setText(global::getSettingsValue("userNameDB", "settings").toString());
    ui->dbPassword->setText(password);
}

void SettingsWindow::saveSettings()
{
    global::setSettingsValue("language", ui->languageSelection->currentText(), "settings");
}

void SettingsWindow::on_saveSettingsButton_clicked()
{
    saveUserData();

    if (!db.isOpen())
        setSettingsDb();

    QDialog::close();

    if (userError == true)
        QMessageBox::information(this, tr("Уведомление"), tr("Такого пользователя не существует!\nДругие настройки успешно сохранены!"), QMessageBox::Ok);
    else
        QMessageBox::information(this, tr("Уведомление"), tr("Настройки успешно сохранены!"), QMessageBox::Ok);
}

/**
 * checks whether the user exists then saves user settings to register
 * and passes params to StartWindow
 */
void SettingsWindow::saveUserData()
{
    QString userFirstName = ui->userFirstName->text();
    QString userSecondName = ui->userSecondName->text();
    QString userPosition = ui->userPosition->text();

    global::setSettingsValue("userLogin", ui->userLogin->text(), "settings");
    QByteArray userPassword;
    userPassword.append(ui->userPassword->text().toLatin1());
    global::setSettingsValue("userPassword", userPassword.toBase64(), "settings");
    global::setSettingsValue("userFirstName", userFirstName, "settings");
    global::setSettingsValue("userSecondName", userSecondName, "settings");
    global::setSettingsValue("userThirdName", ui->userThirdName->text(), "settings");
    global::setSettingsValue("userPosition", userPosition, "settings");

    QSqlQuery queryCheckUser(db);

    queryCheckUser.prepare("SELECT user_login, user_password FROM users_table WHERE (user_login = '" + ui->userLogin->text() + "' AND user_password = '" + ui->userPassword->text() + "')");
    queryCheckUser.exec();

    if (queryCheckUser.first() == 0)
        userError = true;
    else
        userError = false;

    QString userFSname = userFirstName.append(" " + userSecondName);

    emit userData(userFSname, userPosition);
}

void SettingsWindow::setSettingsDb()
{
    global::setSettingsValue("hostName", ui->hostName->text(), "settings");
    global::setSettingsValue("databaseName", ui->dbName->text(), "settings");
    global::setSettingsValue("userNameDB", ui->userName->text(), "settings");
    QByteArray password;
    password.append(ui->dbPassword->text().toLatin1());
    global::setSettingsValue("passwordDB", password.toBase64(), "settings");

    openDb();
}

void SettingsWindow::openDb()
{
    db.setHostName(ui->hostName->text());
    db.setDatabaseName(ui->dbName->text());
    db.setUserName(ui->userName->text());
    db.setPassword(ui->dbPassword->text());
    db.setPort(3306);
    db.open();
}
