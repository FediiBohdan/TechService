#include "AddWorker.h"
#include "ui_AddWorker.h"

AddWorker::AddWorker(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddWorker)
{
    ui->setupUi(this);

    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowFlags(windowFlags() & Qt::WindowMinimizeButtonHint);

    staffDB = QSqlDatabase::addDatabase("QSQLITE");
    staffDB.setDatabaseName("D:\\Diploma\\CRM_AutoService\\ServiceStationDB.db");
    staffDB.open();
}

AddWorker::~AddWorker()
{
    delete ui;
}

void AddWorker::on_saveWorkerButton_clicked()
{
    QSqlQuery query(staffDB);

    QString workerFMLname = ui->workerFMLname->text();
    QString workerPosition = ui->workerPosition->text();
    QString hourlyPayment = ui->hourlyPayment->text();
    QString serviceNumber = ui->serviceNumber->text();

    query.prepare("INSERT INTO StaffTable (staff_FML_name, staff_position, hour_payment, service_number)"
                  "VALUES(?, ?, ?, ?)");

    query.addBindValue(workerFMLname);
    query.addBindValue(workerPosition);
    query.addBindValue(hourlyPayment);
    query.addBindValue(serviceNumber);
    query.exec();

    close();

    QMessageBox::information(this, tr("Уведомление"), tr("Сотрудник успешно добавлен!"), QMessageBox::Ok);
}
