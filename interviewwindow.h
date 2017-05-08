#ifndef INTERVIEWWINDOW_H
#define INTERVIEWWINDOW_H

#include <QWidget>
#include <QDialog>
#include <QtSql>
#include <QCompleter>
#include <QTabWidget>
#include <QTableWidget>

namespace Ui {
class interviewwindow;
}

class interviewwindow : public QDialog
{
    Q_OBJECT

public:
    explicit interviewwindow(QWidget *parent = 0, QSqlDatabase db1 = QSqlDatabase());
    ~interviewwindow();

private slots:
    int doInterview();

    void on_add_contr_clicked();
    //void on_add_button_clicked();

    void on_cansel_button_2_clicked();

    void on_interview_sort_bt_clicked();
    void on_search_contr_to_st_bt_clicked();
    void on_refresh_bt_clicked();



private:
    Ui::interviewwindow *ui;
    QSqlDatabase db;
    int fillTable(QTableWidget *, QString);

protected:
     void paintEvent(QPaintEvent *);

protected slots:
     //int add_contractor();
     int add_contractor_and_int_do();

};

#endif // INTERVIEWWINDOW_H
