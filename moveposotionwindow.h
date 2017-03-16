#ifndef MOVEPOSOTIONWINDOW_H
#define MOVEPOSOTIONWINDOW_H

#include <QWidget>
#include <QDialog>
#include <QtSql>
#include <QCompleter>

namespace Ui {
class moveposotionwindow;
}

class moveposotionwindow : public QDialog
{
    Q_OBJECT

public:
    explicit moveposotionwindow(QWidget *parent = 0, QSqlDatabase db1 = QSqlDatabase());
    ~moveposotionwindow();

private:
    Ui::moveposotionwindow *ui;
    QSqlDatabase db;
};

#endif // MOVEPOSOTIONWINDOW_H
