#ifndef CLIENTTABLE_H
#define CLIENTTABLE_H

#include <QDialog>

namespace Ui {
class ClientTable;
}

class ClientTable : public QDialog
{
    Q_OBJECT

public:
    explicit ClientTable(QWidget *parent = nullptr);
    ~ClientTable();

private:
    Ui::ClientTable *ui;
};

#endif // CLIENTTABLE_H
