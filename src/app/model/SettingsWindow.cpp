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

    ui->serviceComboBox->addItems(QStringList() << "Среднефонтанская, 30А (Приморский р-н)" << "Платонова, 56 (Малиновский р-н)" << "Архитекторская, 28 (Киевский р-н)");

    loadSettings();
    setUserInfo();
}

SettingsWindow::~SettingsWindow()
{
    delete ui;
}

/**
 * Event on language change.
 */
void SettingsWindow::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::LanguageChange)
        ui->retranslateUi(this);
}

/**
 * Sets user settings to window fields.
 */
void SettingsWindow::setUserInfo()
{
    QString registerUserFirstName = global::getSettingsValue("userFirstName", "settings").toString();
    QString registerUserSecondName = global::getSettingsValue("userSecondName", "settings").toString();
    QString registerUserService = global::getSettingsValue("userService", "settings").toString();
    QString registerUserPosition = global::getSettingsValue("userPosition", "settings").toString();
    QString registerUserLogin = global::getSettingsValue("userLogin", "settings").toString();
    QByteArray byteUserPassword = global::getSettingsValue("userPassword", "settings").toByteArray();
    QString userPassword = QString(QByteArray::fromBase64(byteUserPassword));

    if (!registerUserFirstName.isEmpty())
        ui->userFirstName->setText(registerUserFirstName);

    if (!registerUserSecondName.isEmpty())
        ui->userSecondName->setText(registerUserSecondName);

    if (!registerUserService.isEmpty())
    {
        if (registerUserService == "Среднефонтанская, 30А (Приморский р-н)")
            ui->serviceComboBox->setCurrentIndex(0);
        else if (registerUserService == "Платонова, 56 (Малиновский р-н)")
            ui->serviceComboBox->setCurrentIndex(1);
        else if (registerUserService == "Архитекторская, 28 (Киевский р-н)")
            ui->serviceComboBox->setCurrentIndex(2);
    }

    if (!registerUserPosition.isEmpty())
        ui->userPosition->setText(registerUserPosition);

    if (!registerUserLogin.isEmpty())
        ui->userLogin->setText(registerUserLogin);

    if (!userPassword.isEmpty())
        ui->userPassword->setText(userPassword);
}

/**
 * Sets new translation file if UI language was changed.
 */
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

/**
 * Sets DB settings to window fields.
 */
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

/**
 * Sets UI language settings to register.
 */
void SettingsWindow::saveSettings()
{
    global::setSettingsValue("language", ui->languageSelection->currentText(), "settings");
}

/**
 * Calls saveUserData().
 */
void SettingsWindow::on_saveSettingsButton_clicked()
{
    saveUserData();

    if (!db.isOpen())
        setSettingsDb();   

    if (userError == true)
        QMessageBox::information(this, tr("Уведомление"), tr("Такого пользователя не существует. Проверьте логин и пароль!\nДругие настройки успешно сохранены!"), QMessageBox::Ok);
    else
    {
        QDialog::close();

        QMessageBox::information(this, tr("Уведомление"), tr("Настройки успешно сохранены!"), QMessageBox::Ok);
    }
}

/**
 * Checks whether the user exists, then saves user settings to register
 * and passes parameters to StartWindow.
 */
void SettingsWindow::saveUserData()
{
    QString userFirstName = ui->userFirstName->text();
    QString userSecondName = ui->userSecondName->text();
    QString userPosition = ui->userPosition->text();

    QSqlQuery queryCheckUser(db);
    queryCheckUser.prepare("SELECT user_login, user_password FROM users_table WHERE (user_login = '" + ui->userLogin->text() + "' AND user_password = '" + ui->userPassword->text() + "')");
    queryCheckUser.exec();

    if (queryCheckUser.first() == true)
        userError = false;
    else
    {
        userError = true;
        ui->userLogin->setText(NULL);
        ui->userPassword->setText(NULL);
    }

    global::setSettingsValue("userLogin", ui->userLogin->text(), "settings");
    QByteArray userPassword;
    userPassword.append(ui->userPassword->text().toLatin1());
    global::setSettingsValue("userPassword", userPassword.toBase64(), "settings");
    global::setSettingsValue("userFirstName", userFirstName, "settings");
    global::setSettingsValue("userSecondName", userSecondName, "settings");
    global::setSettingsValue("userService", ui->serviceComboBox->currentText(), "settings");
    global::setSettingsValue("userPosition", userPosition, "settings");

    QString userFSname = userFirstName.append(" " + userSecondName);

    emit userData(userFSname, userPosition);
    emit updateUser(true);
}

/**
 * If DB is not opened, saves new connection settings
 * and calls openDb().
 */
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

/**
 * Opens db.
 */
void SettingsWindow::openDb()
{
    db.setHostName(ui->hostName->text());
    db.setDatabaseName(ui->dbName->text());
    db.setUserName(ui->userName->text());
    db.setPassword(ui->dbPassword->text());
    db.setPort(3306);
    db.open();
}
