#ifndef ADDABSENCEWINDOW_H
#define ADDABSENCEWINDOW_H

#include <QWidget>
#include <QDialog>
#include <QtSql>
#include <QCompleter>
#include <QTabWidget>
#include <QTableWidget>


namespace Ui {
class AddAbsenceWindow;
}

class AddAbsenceWindow : public QDialog
{
    Q_OBJECT

public:
    explicit AddAbsenceWindow(QWidget *parent = 0, QSqlDatabase db1 = QSqlDatabase());
    ~AddAbsenceWindow();

private slots:
    int AddAbsence();

    void on_absence_sort_bt_clicked();

    void on_search_contr_to_st_bt_clicked();

    void on_refresh_bt_clicked();

private:
    Ui::AddAbsenceWindow *ui;
    QSqlDatabase db;
    int fillTable(QTableWidget *, QString);


protected:
     void paintEvent(QPaintEvent *);

};

#endif // ADDABSENCEWINDOW_H
