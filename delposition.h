#ifndef DELPOSITION_H
#define DELPOSITION_H

#include <QWidget>
#include <QDialog>
#include <QtSql>
#include <QCompleter>

namespace Ui {
class DelPosition;
}

class DelPosition : public QDialog
{
    Q_OBJECT

public:
    explicit DelPosition(QWidget *parent = 0, QSqlDatabase db1 = QSqlDatabase());
    ~DelPosition();

private:
    Ui::DelPosition *ui;
    QSqlDatabase db;
};

#endif // DELPOSITION_H
