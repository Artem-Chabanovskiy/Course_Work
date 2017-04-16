#ifndef MOVEPOSOTIONWINDOW_H
#define MOVEPOSOTIONWINDOW_H

#include <QWidget>
#include <QDialog>
#include <QtSql>
#include <QCompleter>
#include <QTabWidget>
#include <QTableWidget>

namespace Ui {
class moveposotionwindow;
}

class moveposotionwindow : public QDialog
{
    Q_OBJECT

public:
    explicit moveposotionwindow(QWidget *parent = 0, QSqlDatabase db1 = QSqlDatabase());
    ~moveposotionwindow();

private slots:
    int moveStaff();
    int salary_input();

    void on_position_move_sort_bt_clicked();

    void on_search_contr_to_st_bt_clicked();

    void on_refresh_bt_clicked();

private:
    Ui::moveposotionwindow *ui;
    QSqlDatabase db;
    int fillTable(QTableWidget *, QString);
    QStringList slContr;
};

#endif // MOVEPOSOTIONWINDOW_H
