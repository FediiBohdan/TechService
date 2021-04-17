#include "StartWindow.h"
#include "ui_StartWindow.h"

QElapsedTimer elapsedTimer;
QTimer *timer = new QTimer();
auto countdown = QTime(8, 0, 0);

StartWindow::StartWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StartWindow)
{
    ui->setupUi(this);

    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowFlags(windowFlags() & Qt::WindowMinimizeButtonHint);

    StartWindow::showMaximized();

    ui->positionLabel->hide();

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

    elapsedTimer.start();
    connect(timer, SIGNAL(timeout()), this, SLOT(showTime()));
    timer->start(1000);

    connect(ui->updateButton, &QAbstractButton::clicked, this, &StartWindow::updateTasksList);

    loadTasksList();
}

StartWindow::~StartWindow()
{
    delete ui;
}

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

void StartWindow::setUserData(const QString &userFSname, const QString &userPosition)
{
    ui->positionLabel->show();

    ui->nameLabel->setText(userFSname);
    ui->positionLabel->setText(userPosition);
}

void StartWindow::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::LanguageChange)
        ui->retranslateUi(this);
}

void StartWindow::showTime()
{
    auto elapsed = elapsedTimer.elapsed();
    auto counter = countdown.addMSecs(-elapsed);
    QString timestr = counter.toString("hh:mm:ss");
    ui->lcdNumber->display(timestr);
}

void StartWindow::loadTasksList()
{
    queryModel = new QSqlQueryModel(this);

    QString queryString = "SELECT id_to_do_list, content FROM tasks_table";

    queryModel->setQuery(queryString, listTasksTable);

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

QWidget* StartWindow::addCheckBoxCompleted(int rowIndex)
{
    QWidget *widget = new QWidget(this);
    QHBoxLayout *layout = new QHBoxLayout(widget);
    QCheckBox *checkBox = new QCheckBox(widget);

    layout->addWidget(checkBox, 0, Qt::AlignCenter);

    queryModelCheckBox = new QSqlQueryModel(this);

    QString queryStringCheckBox = "SELECT is_fulfilled FROM tasks_table";

    queryModelCheckBox->setQuery(queryStringCheckBox, listTasksTable);

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

void StartWindow::on_desktopButton_clicked()
{
    // does not work properly
//    if (sparePartsTable->isVisible())
//    {
//        connect(this, &StartWindow::closeAllWindowsExceptCurrent, sparePartsTable, &ListSparePart::closeWindow);
//        emit closeAllWindowsExceptCurrent(true);
//    }
//    if (addOrder->isVisible())
//    {
//        connect(this, &StartWindow::closeAllWindowsExceptCurrent, addOrder, &AddOrder::closeWindow);
//        emit closeAllWindowsExceptCurrent(true);
//    }

    qDebug() << sparePartsTable->isVisible();
}

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

void StartWindow::on_settingsButton_clicked()
{
    settingsWindow = new SettingsWindow;
    settingsWindow->show();
    connect(settingsWindow, &SettingsWindow::userData, this, &StartWindow::setUserData);
    connect(settingsWindow, &SettingsWindow::translate, this, &StartWindow::translateUI);
    settingsWindow->setAttribute(Qt::WA_DeleteOnClose);
}

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
