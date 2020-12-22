#ifndef SPAREPARTSTABLE_H
#define SPAREPARTSTABLE_H

#include <QDialog>

namespace Ui {
class SparePartsTable;
}

class SparePartsTable : public QDialog
{
    Q_OBJECT

public:
    explicit SparePartsTable(QWidget *parent = nullptr);
    ~SparePartsTable();

private:
    Ui::SparePartsTable *ui;
};

#endif // SPAREPARTSTABLE_H
