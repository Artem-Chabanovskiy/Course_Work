#ifndef REJECTCONRINTERVIEW_H
#define REJECTCONRINTERVIEW_H

#include <QWidget>
#include <QDialog>
#include <QtSql>
#include <QCompleter>
#include <QTabWidget>
#include <QTableWidget>


namespace Ui {
class RejectConrInterview;
}

class RejectConrInterview : public QDialog
{
    Q_OBJECT

public:
    explicit RejectConrInterview(QWidget *parent = 0, QSqlDatabase db1 = QSqlDatabase());
    ~RejectConrInterview();

private slots:
    int ContrRejectInt();

    void on_search_contr_to_st_bt_clicked();

    void on_refresh_bt_clicked();

private:
    Ui::RejectConrInterview *ui;
    QSqlDatabase db;
    int fillTable(QTableWidget *, QString);

protected:
     void paintEvent(QPaintEvent *);

};

#endif // REJECTCONRINTERVIEW_H
