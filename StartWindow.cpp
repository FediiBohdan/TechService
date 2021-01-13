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

   // connect(ui->addButton, &QAbstractButton::clicked, this, &StartWindow::onAdd);
   // connect(ui->removeButton, &QAbstractButton::clicked, this, &StartWindow::onRemove);

    loadTaskList();
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

void StartWindow::loadTaskList()
{
    queryModel = new QSqlQueryModel(this);

    QString queryString;

    queryString = "SELECT id_to_do_list, content FROM TasksTable";

    queryModel->setQuery(queryString, listTasksTable);

    queryModel->setHeaderData(0, Qt::Horizontal, tr("id"));
    queryModel->insertColumn(1);
    queryModel->setHeaderData(1, Qt::Horizontal, tr("Выполнено"));
    queryModel->setHeaderData(2, Qt::Horizontal, tr("Содержание"));

    // "completed" tableView
    ui->completedTasksTableView->setModel(queryModel);
    ui->completedTasksTableView->setColumnHidden(0, true);
    ui->completedTasksTableView->horizontalHeader()->setDefaultSectionSize(maximumWidth());
    ui->completedTasksTableView->resizeColumnsToContents();

    // "not completed" tableView
    ui->notCompletedTasksTableView->setModel(queryModel);
    ui->notCompletedTasksTableView->setColumnHidden(0, true);
    ui->notCompletedTasksTableView->horizontalHeader()->setDefaultSectionSize(maximumWidth());
    ui->notCompletedTasksTableView->resizeColumnsToContents();

    for (qint32 row_index = 0; row_index < ui->completedTasksTableView->model()->rowCount(); ++row_index)
    {
        ui->completedTasksTableView->setIndexWidget(queryModel->index(row_index, 1), addCheckBoxCompleted(row_index));
        ui->completedTasksTableView->setRowHeight(row_index, 50);
    }

    for (qint32 row_index = 0; row_index < ui->notCompletedTasksTableView->model()->rowCount(); ++row_index)\
    {
        ui->notCompletedTasksTableView->setIndexWidget(queryModel->index(row_index, 1), addCheckBoxCompleted(row_index));
        ui->notCompletedTasksTableView->setRowHeight(row_index, 50);
    }

    ui->notCompletedTasksTableView->verticalHeader()->hide();
    ui->completedTasksTableView->verticalHeader()->hide();
}

QWidget* StartWindow::addCheckBoxCompleted(qint32 row_index)
{
    QWidget *widget = new QWidget(this);
    QHBoxLayout *layout = new QHBoxLayout(widget);
    QCheckBox *checkBox = new QCheckBox(widget);

    layout->addWidget(checkBox, 0, Qt::AlignCenter);

    queryModelCheckBox = new QSqlQueryModel(this);

    QString queryStringCheckBox;

    queryStringCheckBox = "SELECT is_fulfilled FROM TasksTable";

    queryModelCheckBox->setQuery(queryStringCheckBox, listTasksTable);

    QString isFulfilled = queryModelCheckBox->data(queryModelCheckBox->index(row_index, 0), Qt::EditRole).toString();

    if (isFulfilled == "1")
    {
        checkBox->setChecked(true);
        ui->notCompletedTasksTableView->setRowHidden(row_index, true);
    }
    else
        ui->completedTasksTableView->setRowHidden(row_index, true);

    QString column = "is_fulfilled";
    QString id = queryModel->data(queryModel->index(row_index, 0), Qt::EditRole).toString();

    connect(checkBox, &QAbstractButton::pressed, this, &StartWindow::checkBoxStateChanged);

    checkBox->setProperty("checkBox", QVariant::fromValue(checkBox));
    checkBox->setProperty("id",       QVariant::fromValue(id));
    checkBox->setProperty("column",   QVariant::fromValue(column));

    return widget;
}

void StartWindow::checkBoxStateChanged()
{
    QString id = sender()->property("id").value<QString>();
    QString column = sender()->property("column").value<QString>();
    QCheckBox *checkBox = sender()->property("checkBox").value<QCheckBox*>();

    QSqlQuery query(listTasksDB);

    if (!checkBox->isChecked() && column == "is_fulfilled")
    {
        checkBox->setChecked(true);

        query.prepare("UPDATE TasksTable SET is_fulfilled = 1 WHERE id_to_do_list = ?");
        query.addBindValue(id);
        query.exec();
    }

    //loadTaskList();
}

void StartWindow::onAdd()
{
//    ui->notCompletedListView->model()->insertRow(ui->notCompletedListView->model()->rowCount());
//    QModelIndex qIndex = ui->notCompletedListView->model()->index(ui->notCompletedListView->model()->rowCount() - 1, 0);

//    ui->notCompletedListView->edit(qIndex);
}

void StartWindow::onRemove()
{
//    QModelIndex qIndex = ui->notCompletedListView->currentIndex();
//    ui->notCompletedListView->model()->removeRow(qIndex.row());
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
