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

    StartWindow::showNormal();
    StartWindow::showMaximized();

    elapsedTimer.start();
    connect(timer, SIGNAL(timeout()), this, SLOT(showTime()));
    timer->start(1000);

    QDir tempDirDB = QDir::currentPath(); tempDirDB.cdUp(); QString dirDB = tempDirDB.path();

    listTasksDB = QSqlDatabase::addDatabase("QSQLITE");
    listTasksDB.setDatabaseName(dirDB + "\\CRM_AutoService\\ServiceStationDB.db");
    listTasksDB.open();

    connect(ui->updateButton, &QAbstractButton::clicked, this, &StartWindow::updateTasksList);

    loadTasksList();
}

StartWindow::~StartWindow()
{
    delete ui;
}

void StartWindow::translateUI(int translate)
{
    if (translate == 0)
    {
        translator.load(":/translations/russian.qm");
        qApp->installTranslator(&translator);
    }
    else if (translate == 1)
    {
        translator.load(":/translations/ukrainian.qm");
        qApp->installTranslator(&translator);
    }
    else if (translate == 2)
    {
        translator.load(":/translations/english.qm");
        qApp->installTranslator(&translator);
    }
}

void StartWindow::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::LanguageChange)
        ui->retranslateUi(this);
}

void StartWindow::showTime(){
    auto elapsed = elapsedTimer.elapsed();
    auto counter = countdown.addMSecs(-elapsed);
    QString timestr = counter.toString("hh:mm:ss");
    ui->lcdNumber->display(timestr);
}

void StartWindow::loadTasksList()
{
    queryModel = new QSqlQueryModel(this);

    QString queryString = "SELECT id_to_do_list, content FROM TasksTable";

    queryModel->setQuery(queryString, listTasksTable);

    queryModel->setHeaderData(0, Qt::Horizontal, tr("id"));
    queryModel->insertColumn(1);
    queryModel->setHeaderData(1, Qt::Horizontal, tr("Выполнено"));
    queryModel->setHeaderData(2, Qt::Horizontal, tr("Содержание"));

    // tableView with completed tasks
    ui->completedTasksTableView->setModel(queryModel);
    ui->completedTasksTableView->setColumnHidden(0, true);
    ui->completedTasksTableView->horizontalHeader()->setDefaultSectionSize(maximumWidth());
    ui->completedTasksTableView->resizeColumnsToContents();

    // tableView with not completed tasks
    ui->notCompletedTasksTableView->setModel(queryModel);
    ui->notCompletedTasksTableView->setColumnHidden(0, true);
    ui->notCompletedTasksTableView->horizontalHeader()->setDefaultSectionSize(maximumWidth());
    ui->notCompletedTasksTableView->resizeColumnsToContents();

    for (int rowIndex = 0; rowIndex < ui->completedTasksTableView->model()->rowCount(); ++rowIndex)
        ui->completedTasksTableView->setIndexWidget(queryModel->index(rowIndex, 1), addCheckBoxCompleted(rowIndex));

    for (int rowIndex = 0; rowIndex < ui->notCompletedTasksTableView->model()->rowCount(); ++rowIndex)
        ui->notCompletedTasksTableView->setIndexWidget(queryModel->index(rowIndex, 1), addCheckBoxCompleted(rowIndex));

    ui->notCompletedTasksTableView->verticalHeader()->hide();
    ui->completedTasksTableView->verticalHeader()->hide();
    ui->notCompletedTasksTableView->resizeRowsToContents();
    ui->completedTasksTableView->resizeRowsToContents();
}

QWidget* StartWindow::addCheckBoxCompleted(int rowIndex)
{
    QWidget *widget = new QWidget(this);
    QHBoxLayout *layout = new QHBoxLayout(widget);
    QCheckBox *checkBox = new QCheckBox(widget);

    layout->addWidget(checkBox, 0, Qt::AlignCenter);

    queryModelCheckBox = new QSqlQueryModel(this);

    QString queryStringCheckBox = "SELECT is_fulfilled FROM TasksTable";

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

    QSqlQuery query(listTasksDB);

    if (!checkBox->isChecked())
    {
        checkBox->setChecked(true);

        query.prepare("UPDATE TasksTable SET is_fulfilled = 1 WHERE id_to_do_list = ?");
        query.addBindValue(id);
        query.exec();
    }
    else if (checkBox->isChecked())
    {
        checkBox->setChecked(false);

        query.prepare("UPDATE TasksTable SET is_fulfilled = 0 WHERE id_to_do_list = ?");
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

void StartWindow::on_statisticsButton_clicked()
{

}

void StartWindow::on_settingsButton_clicked()
{
    settingsWindow = new SettingsWindow;
    settingsWindow->show();
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
