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

private:
    Ui::RejectConrInterview *ui;
    QSqlDatabase db;
    int fillTable(QTableWidget *, QString);
    QStringList slContr;
};

#endif // REJECTCONRINTERVIEW_H
