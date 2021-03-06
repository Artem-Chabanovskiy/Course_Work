#ifndef ADDCONTRACTORTOSTAFF_H
#define ADDCONTRACTORTOSTAFF_H

#include <QWidget>
#include <QDialog>
#include <QtSql>
#include <QCompleter>
#include <QTableWidget>

namespace Ui {
class AddContractorToStaff;
}

class AddContractorToStaff : public QDialog
{
    Q_OBJECT

public:
    explicit AddContractorToStaff(QWidget *parent = 0, QSqlDatabase db1 = QSqlDatabase());
    ~AddContractorToStaff();

private slots:

    int pushContrToStaff();
    //search and refresh buttons
    void on_addcontr_sort_bt_clicked();   
    void on_search_contr_to_st_bt_clicked();
    void on_refresh_bt_clicked();
    //additional buttons
    void on_add_to_staff_first_bt_clicked();
    void on_return_bt_clicked();

private:
    Ui::AddContractorToStaff *ui;
    QSqlDatabase db;
    int fillTable(QTableWidget *, QString);

protected:
     void paintEvent(QPaintEvent *);
};

#endif // ADDCONTRACTORTOSTAFF_H
