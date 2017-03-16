#ifndef ADDCONTRACTORTOSTAFF_H
#define ADDCONTRACTORTOSTAFF_H

#include <QWidget>
#include <QDialog>
#include <QtSql>
#include <QCompleter>

namespace Ui {
class AddContractorToStaff;
}

class AddContractorToStaff : public QDialog
{
    Q_OBJECT

public:
    explicit AddContractorToStaff(QWidget *parent = 0, QSqlDatabase db1 = QSqlDatabase());
    ~AddContractorToStaff();

private:
    Ui::AddContractorToStaff *ui;
    QSqlDatabase db;
};

#endif // ADDCONTRACTORTOSTAFF_H
