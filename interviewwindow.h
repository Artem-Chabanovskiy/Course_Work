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

    void on_interview_sort_bt_clicked();

    void on_search_contr_to_st_bt_clicked();

    void on_refresh_bt_clicked();

private:
    Ui::interviewwindow *ui;
    QSqlDatabase db;
    int fillTable(QTableWidget *, QString);

protected:
     void paintEvent(QPaintEvent *);

};

#endif // INTERVIEWWINDOW_H
