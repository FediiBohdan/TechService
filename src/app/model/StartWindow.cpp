#include "StartWindow.h"
#include "ui_StartWindow.h"

StartWindow::StartWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StartWindow)
{
    ui->setupUi(this);

    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowFlags(windowFlags() & Qt::WindowMinimizeButtonHint);

    StartWindow::showMaximized();

    ui->positionLabel->hide();

    connect(ui->updateButton, &QAbstractButton::clicked, this, &StartWindow::updateTasksList);
    connect(ui->ordersUpdateButton, &QAbstractButton::clicked, this, &StartWindow::updateOrdersList);

    loadTasksList();
    loadOrdersList();
    loadPersonalData();
    loadUserSettings();
}

StartWindow::~StartWindow()
{
    delete ui;
}

/**
 * Sets received parameters and sets new UI translation file.
 */
void StartWindow::translateUI(const QString &language)
{
    if (!translator.isEmpty())
        qApp->removeTranslator(&translator);

    if (language == "russian")
        translator.load(":/translations/russian.qm");
    else if (language == "ukrainian")
        translator.load(":/translations/ukrainian.qm");
    else if (language == "english")
        translator.load(":/translations/english.qm");

    qApp->installTranslator(&translator);
}

/**
 * Event on language change.
 */
void StartWindow::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::LanguageChange)
        ui->retranslateUi(this);
}

/**
 * Sets new user data from SettingsWindow.
 */
void StartWindow::setUserData(const QString &userFSname, const QString &userPosition)
{
    ui->positionLabel->show();

    ui->nameLabel->setText(userFSname);
    ui->positionLabel->setText(userPosition);
}

/**
 * Loads personal data from register.
 */
void StartWindow::loadPersonalData()
{
    QString registerUserFirstName = global::getSettingsValue("userFirstName", "settings").toString();
    QString registerUserSecondName = global::getSettingsValue("userSecondName", "settings").toString();
    QString registerUserPosition = global::getSettingsValue("userPosition", "settings").toString();

    if ((ui->positionLabel->isHidden()) && (!registerUserFirstName.isEmpty()))
    {
        QString userFSname = registerUserFirstName.append(" " + registerUserSecondName);

        ui->positionLabel->show();
        ui->nameLabel->setText(userFSname);
        ui->positionLabel->setText(registerUserPosition);
    }
}

/**
 * Loads user settings for granting access.
 */
void StartWindow::loadUserSettings()
{
    QString userLogin = global::getSettingsValue("userLogin", "settings").toString();
    QString userPassword = global::getSettingsValue("userPassword", "settings").toString();
    int pos = 0;

    QRegularExpression chiefRegexp("^[1][0-9]{3}$");
    QRegularExpressionValidator chiefValidator(chiefRegexp, this);

    QRegularExpression managerRegexp("^[2][0-9]{3}$");
    QRegularExpressionValidator managerValidator(managerRegexp, this);

    QRegularExpression techEmployeeRegexp("^[3][0-9]{3}$");
    QRegularExpressionValidator techEmployeeValidator(techEmployeeRegexp, this);

    if (chiefValidator.validate(userLogin, pos) == QValidator::Acceptable)
    {
        ui->staffButton->setEnabled(true);
        ui->updateButton->setEnabled(true);
        ui->clientsButton->setEnabled(true);
        ui->todolistButton->setEnabled(true);
        ui->ordersTableView->setEnabled(true);
        ui->catalogueButton->setEnabled(true);
        ui->analyticsButton->setEnabled(true);
        ui->ordersUpdateButton->setEnabled(true);
        ui->orderFormationButton->setEnabled(true);
    }
    else if (managerValidator.validate(userLogin, pos) == QValidator::Acceptable)
    {        
        ui->staffButton->setEnabled(true);
        ui->updateButton->setEnabled(true);
        ui->clientsButton->setEnabled(true);
        ui->todolistButton->setEnabled(true);
        ui->catalogueButton->setEnabled(true);
        ui->ordersTableView->setEnabled(true);
        ui->analyticsButton->setEnabled(false);
        ui->ordersUpdateButton->setEnabled(true);
        ui->orderFormationButton->setEnabled(true);
    }
    else if (techEmployeeValidator.validate(userLogin, pos) == QValidator::Acceptable)
    {
        ui->updateButton->setEnabled(true);
        ui->staffButton->setEnabled(false);
        ui->clientsButton->setEnabled(true);
        ui->todolistButton->setEnabled(true);
        ui->catalogueButton->setEnabled(true);
        ui->ordersTableView->setEnabled(true);
        ui->analyticsButton->setEnabled(false);
        ui->ordersUpdateButton->setEnabled(true);
        ui->orderFormationButton->setEnabled(true);
    }
    else
    {
        ui->staffButton->setEnabled(false);
        ui->updateButton->setEnabled(false);
        ui->clientsButton->setEnabled(false);
        ui->todolistButton->setEnabled(false);
        ui->catalogueButton->setEnabled(false);
        ui->analyticsButton->setEnabled(false);
        ui->ordersTableView->setEnabled(false);
        ui->ordersUpdateButton->setEnabled(false);
        ui->orderFormationButton->setEnabled(false);
    }

    if (!userLogin.isEmpty() && !userPassword.isEmpty())
    {
        QPixmap pixmap(":/images/loggedUser.png");
        ui->accountLogIn->setIcon(pixmap);
    }
    else
    {
        QPixmap pixmap(":/images/noUser.png");
        ui->accountLogIn->setIcon(pixmap);
        ui->nameLabel->setText(tr("Войдите в аккаунт!"));
        ui->positionLabel->hide();
    }

    updateTasksList();
}

/**
 * Loads task list to viewTable.
 */
void StartWindow::loadTasksList()
{
    queryModel = new QSqlQueryModel(this);

    QString userLogin = global::getSettingsValue("userLogin", "settings").toString();

    QString queryString = "SELECT id_to_do_list, content FROM tasks_table WHERE user = " + userLogin;

    queryModel->setQuery(queryString);

    queryModel->setHeaderData(0, Qt::Horizontal, tr("id"));
    queryModel->insertColumn(1);
    queryModel->setHeaderData(1, Qt::Horizontal, tr("Выполнено"));
    queryModel->setHeaderData(2, Qt::Horizontal, tr("Содержание"));

    // tableView with completed tasks
    ui->completedTasksTableView->setModel(queryModel);
    ui->completedTasksTableView->setColumnHidden(0, true);
    ui->completedTasksTableView->verticalHeader()->hide();
    ui->completedTasksTableView->setColumnWidth(1, 95);
    ui->completedTasksTableView->setColumnWidth(2, 287);
    ui->completedTasksTableView->horizontalHeader()->setSectionsClickable(false);

    // tableView with not completed tasks
    ui->notCompletedTasksTableView->setModel(queryModel);
    ui->notCompletedTasksTableView->setColumnHidden(0, true);
    ui->notCompletedTasksTableView->verticalHeader()->hide();
    ui->notCompletedTasksTableView->setColumnWidth(1, 95);
    ui->notCompletedTasksTableView->setColumnWidth(2, 288);
    ui->notCompletedTasksTableView->horizontalHeader()->setSectionsClickable(false);

    for (int rowIndex = 0; rowIndex < ui->completedTasksTableView->model()->rowCount(); ++rowIndex)
        ui->completedTasksTableView->setIndexWidget(queryModel->index(rowIndex, 1), addCheckBoxCompleted(rowIndex));

    for (int rowIndex = 0; rowIndex < ui->notCompletedTasksTableView->model()->rowCount(); ++rowIndex)
        ui->notCompletedTasksTableView->setIndexWidget(queryModel->index(rowIndex, 1), addCheckBoxCompleted(rowIndex));

    ui->completedTasksTableView->resizeRowsToContents();
    ui->notCompletedTasksTableView->resizeRowsToContents();
}

/**
 * Adds checkBox widget to tableView.
 * It determines whether the task is completed or not.
 */
QWidget *StartWindow::addCheckBoxCompleted(int rowIndex)
{
    QString userLogin = global::getSettingsValue("userLogin", "settings").toString();

    QWidget *widget = new QWidget(this);
    QHBoxLayout *layout = new QHBoxLayout(widget);
    QCheckBox *checkBox = new QCheckBox(widget);

    layout->addWidget(checkBox, 0, Qt::AlignCenter);

    queryModelCheckBox = new QSqlQueryModel(this);

    QString queryStringCheckBox = "SELECT is_fulfilled FROM tasks_table WHERE user = " + userLogin;

    queryModelCheckBox->setQuery(queryStringCheckBox);

    QString isFulfilled = queryModelCheckBox->data(queryModelCheckBox->index(rowIndex, 0), Qt::EditRole).toString();

    // set checked/unchecked in tableView
    if (isFulfilled == "1")
    {
        checkBox->setChecked(true);
        ui->notCompletedTasksTableView->setRowHidden(rowIndex, true);
    }
    else
        ui->completedTasksTableView->setRowHidden(rowIndex, true);

    connect(checkBox, &QAbstractButton::pressed, this, &StartWindow::checkBoxStateChanged);

    QString id = queryModel->data(queryModel->index(rowIndex, 0), Qt::EditRole).toString();

    checkBox->setProperty("checkBox", QVariant::fromValue(checkBox));
    checkBox->setProperty("id",       QVariant::fromValue(id));

    return widget;
}

/**
 * Processes checkBox state change.
 */
void StartWindow::checkBoxStateChanged()
{
    QString id = sender()->property("id").value<QString>();
    QCheckBox *checkBox = sender()->property("checkBox").value<QCheckBox*>();

    QSqlQuery query(listTasksTable);

    if (!checkBox->isChecked())
    {
        checkBox->setChecked(true);

        query.prepare("UPDATE tasks_table SET is_fulfilled = 1 WHERE id_to_do_list = ?");
        query.addBindValue(id);
        query.exec();
    }
    else if (checkBox->isChecked())
    {
        checkBox->setChecked(false);

        query.prepare("UPDATE tasks_table SET is_fulfilled = 0 WHERE id_to_do_list = ?");
        query.addBindValue(id);
        query.exec();
    }

    updateTasksList();
}

void StartWindow::updateTasksList()
{
    queryModel->setQuery(NULL);

    loadTasksList();
}

/**
 * Loads orders list to tableView.
 */
void StartWindow::loadOrdersList()
{
    QString registerUserService = global::getSettingsValue("userService", "settings").toString();
    QString userLogin = global::getSettingsValue("userLogin", "settings").toString();

    QRegularExpression chiefRegexp("^[1][0-9]{3}$");
    QRegularExpressionValidator chiefValidator(chiefRegexp, this);
    int pos = 0;

    queryOrdersModel = new QSqlQueryModel(this);

    QString queryString = "SELECT id_order, client, contacts, auto_brand, auto_model, service_address, creation_date FROM orders_history ";
    QString searchString;

    if (!(chiefValidator.validate(userLogin, pos) == QValidator::Acceptable))
        searchString.append("WHERE service_address LIKE '%" + registerUserService + "%'");

    queryString.append(searchString);

    queryOrdersModel->setQuery(queryString);

    queryOrdersModel->setHeaderData(0, Qt::Horizontal, tr("id"));
    queryOrdersModel->insertColumn(1);
    queryOrdersModel->setHeaderData(1, Qt::Horizontal, tr("Выполнен"));
    queryOrdersModel->setHeaderData(2, Qt::Horizontal, tr("Клиент"));
    queryOrdersModel->setHeaderData(3, Qt::Horizontal, tr("Контакты"));
    queryOrdersModel->setHeaderData(4, Qt::Horizontal, tr("Марка"));
    queryOrdersModel->setHeaderData(5, Qt::Horizontal, tr("Модель"));
    queryOrdersModel->setHeaderData(6, Qt::Horizontal, tr("Сервис"));
    queryOrdersModel->setHeaderData(7, Qt::Horizontal, tr("Дата создания"));

    ui->ordersTableView->setModel(queryOrdersModel);
    ui->ordersTableView->setColumnHidden(0, true);
    ui->ordersTableView->verticalHeader()->hide();

    ui->ordersTableView->horizontalHeader()->setSectionsClickable(false);

    for (int rowIndex = 0; rowIndex < ui->ordersTableView->model()->rowCount(); ++rowIndex)
        ui->ordersTableView->setIndexWidget(queryOrdersModel->index(rowIndex, 1), addCheckBoxOrderCompleted(rowIndex));

    ui->ordersTableView->horizontalHeader()->setDefaultSectionSize(maximumWidth());
    ui->ordersTableView->resizeColumnsToContents();
    ui->ordersTableView->resizeRowsToContents();
}

/**
 * Adds checkBox widget to tableView.
 * It determines whether the order is completed or not.
 */
QWidget *StartWindow::addCheckBoxOrderCompleted(int rowIndex)
{
    QWidget *widget = new QWidget(this);
    QHBoxLayout *layout = new QHBoxLayout(widget);
    QCheckBox *checkBox = new QCheckBox(widget);

    layout->addWidget(checkBox, 0, Qt::AlignCenter);

    queryOrdersModelCheckBox = new QSqlQueryModel(this);

    queryOrdersModelCheckBox->setQuery("SELECT is_ready FROM orders_history");

    QString isFulfilled = queryOrdersModelCheckBox->data(queryOrdersModelCheckBox->index(rowIndex, 0), Qt::EditRole).toString();

    if (isFulfilled == "1")
        checkBox->setChecked(true);
    else
        checkBox->setChecked(false);

    connect(checkBox, &QAbstractButton::pressed, this, &StartWindow::checkBoxOrderStateChanged);

    QString id = queryOrdersModel->data(queryOrdersModel->index(rowIndex, 0), Qt::EditRole).toString();

    checkBox->setProperty("checkBox", QVariant::fromValue(checkBox));
    checkBox->setProperty("id",       QVariant::fromValue(id));

    return widget;
}

/**
 * Processes checkBox state change.
 */
void StartWindow::checkBoxOrderStateChanged()
{
    QString id = sender()->property("id").value<QString>();
    QCheckBox *checkBox = sender()->property("checkBox").value<QCheckBox*>();

    QSqlQuery query(listOrdersTable);

    if (!checkBox->isChecked())
    {
        checkBox->setChecked(true);

        query.prepare("UPDATE orders_history SET is_ready = 1 WHERE id_order = ?");
        query.addBindValue(id);
        query.exec();
    }
    else if (checkBox->isChecked())
    {
        checkBox->setChecked(false);

        query.prepare("UPDATE orders_history SET is_ready = 0 WHERE id_order = ?");
        query.addBindValue(id);
        query.exec();
    }

    updateOrdersList();
}

void StartWindow::updateOrdersList()
{
    queryOrdersModel->setQuery(NULL);

    loadOrdersList();
}

/**
 * Opens corresponding windows.
 */
void StartWindow::on_orderFormationButton_clicked()
{
    listOrders = new ListOrders;
    listOrders->show();
    listOrders->setAttribute(Qt::WA_DeleteOnClose);
}

void StartWindow::on_clientsButton_clicked()
{
    listClients = new ListClients;
    listClients->show();
    listClients->setAttribute(Qt::WA_DeleteOnClose);
}

void StartWindow::on_staffButton_clicked()
{
    listEmployees = new ListEmployees;
    listEmployees->show();
    listEmployees->setAttribute(Qt::WA_DeleteOnClose);
}

void StartWindow::on_catalogueButton_clicked()
{
    sparePartsTable = new ListSparePart;
    sparePartsTable->show();
    sparePartsTable->setAttribute(Qt::WA_DeleteOnClose);
}

void StartWindow::on_todolistButton_clicked()
{
    listTasks = new ListTasks;
    listTasks->show();
    listTasks->setAttribute(Qt::WA_DeleteOnClose);
}

void StartWindow::on_analyticsButton_clicked()
{
    analyticsDialog = new AnalyticsDialog;
    analyticsDialog->show();
    analyticsDialog->setAttribute(Qt::WA_DeleteOnClose);
}

/**
 * Opens settings window and get signals for processing in slots.
 */
void StartWindow::on_settingsButton_clicked()
{
    settingsWindow = new SettingsWindow;
    settingsWindow->show();
    connect(settingsWindow, &SettingsWindow::userData, this, &StartWindow::setUserData);
    connect(settingsWindow, &SettingsWindow::translate, this, &StartWindow::translateUI);
    connect(settingsWindow, &SettingsWindow::updateUser, this, &StartWindow::loadUserSettings);
    connect(settingsWindow, &SettingsWindow::updateUser, this, &StartWindow::loadOrdersList);
    settingsWindow->setAttribute(Qt::WA_DeleteOnClose);
}

/**
 * Opens web sites.
 */
void StartWindow::on_telegramButton_clicked()
{
    QString telegramLink = "https://web.telegram.org/";
    QDesktopServices::openUrl(QUrl(telegramLink));
}

void StartWindow::on_instagramButton_clicked()
{
    QString instagramLink = "https://www.instagram.com/";
    QDesktopServices::openUrl(QUrl(instagramLink));
}

void StartWindow::on_facebookButton_clicked()
{
    QString facebookLink = "https://www.facebook.com/";
    QDesktopServices::openUrl(QUrl(facebookLink));
}

void StartWindow::on_viberButton_clicked()
{
    QString viberLink = "https://www.viber.com/";
    QDesktopServices::openUrl(QUrl(viberLink));
}

void StartWindow::on_youtubeButton_clicked()
{
    QString youtubeLink = "https://www.youtube.com/";
    QDesktopServices::openUrl(QUrl(youtubeLink));
}
