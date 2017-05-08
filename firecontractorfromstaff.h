#ifndef FIRECONTRACTORFROMSTAFF_H
#define FIRECONTRACTORFROMSTAFF_H

#include <QWidget>
#include <QDialog>
#include <QtSql>
#include <QCompleter>
#include <QTableWidget>

namespace Ui {
class FireContractorFromStaff;
}

class FireContractorFromStaff : public QDialog
{
    Q_OBJECT

public:
    explicit FireContractorFromStaff(QWidget *parent = 0, QSqlDatabase db1 = QSqlDatabase());
    ~FireContractorFromStaff();

private slots:
    int fireContrFromStaff();

    void on_search_contr_from_st_bt_clicked();
    void on_refresh_bt_clicked();

private:
    Ui::FireContractorFromStaff *ui;
    QSqlDatabase db;

    int fillTable(QTableWidget *, QString);

protected:
     void paintEvent(QPaintEvent *);
};

#endif // FIRECONTRACTORFROMSTAFF_H
