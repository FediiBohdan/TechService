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

    QByteArray byte_password = global::getSettingsValue("passwordDB", "settings").toByteArray();
    QString password = QString(QByteArray::fromBase64(byte_password));
    ui->hostName->setText(global::getSettingsValue("hostName", "settings").toString());
    ui->dbName->setText(global::getSettingsValue("databaseName", "settings").toString());
    ui->userName->setText(global::getSettingsValue("userNameDB", "settings").toString());
    ui->dbPassword->setText(password);
    //ui->port->setText(global::getSettingsValue("port_1", "settings").toString());

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
    setSettingsDb();

    if (!m_db.isOpen())
    {
        setSettingsDb();

        QMessageBox::information(this, tr("Предупреждение"), tr("Невозможно подключиться к базам данных!"), QMessageBox::Ok);

        QDialog::close();
    }
    else if (!m_db.isOpen())
    {
        openDb();

        QMessageBox::information(this, tr("Уведомление"), tr("Подключение успешно создано!"), QMessageBox::Ok);
    }

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

void SettingsWindow::setSettingsDb()
{
    global::setSettingsValue("hostName", ui->hostName->text(), "settings");
    global::setSettingsValue("databaseName", ui->dbName->text(), "settings");
    global::setSettingsValue("userNameDB", ui->userName->text(), "settings");
    QByteArray password_1;
    password_1.append(ui->dbPassword->text().toLatin1());
    global::setSettingsValue("passwordDB", password_1.toBase64(), "settings");
    //global::setSettingsValue("port_1", ui->port_1->text(), "settings");
}

void SettingsWindow::setDatabases(const QSqlDatabase& db)
{
    m_db = db;
}

void SettingsWindow::openDb()
{
    m_db.setHostName(ui->hostName->text());
    m_db.setDatabaseName(ui->dbName->text());
    m_db.setUserName(ui->userName->text());
    m_db.setPassword(ui->dbPassword->text());
    m_db.setPort(3306);
    m_db.open();
}
