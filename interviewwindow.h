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

private:
    Ui::interviewwindow *ui;
    QSqlDatabase db;
    int fillTable(QTableWidget *, QString);
};

#endif // INTERVIEWWINDOW_H
