#ifndef INTERVIEWWINDOW_H
#define INTERVIEWWINDOW_H

#include <QWidget>
#include <QDialog>
#include <QtSql>
#include <QCompleter>

namespace Ui {
class interviewwindow;
}

class interviewwindow : public QDialog
{
    Q_OBJECT

public:
    explicit interviewwindow(QWidget *parent = 0, QSqlDatabase db1 = QSqlDatabase());
    ~interviewwindow();

private:
    Ui::interviewwindow *ui;
    QSqlDatabase db;
};

#endif // INTERVIEWWINDOW_H
