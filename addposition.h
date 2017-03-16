#ifndef ADDPOSITION_H
#define ADDPOSITION_H

#include <QWidget>
#include <QDialog>
#include <QtSql>
#include <QCompleter>

namespace Ui {
class AddPosition;
}

class AddPosition : public QDialog
{
    Q_OBJECT

public:
    explicit AddPosition(QWidget *parent = 0, QSqlDatabase db1 = QSqlDatabase());
    ~AddPosition();

private slots:
    int add_position();

private:
    Ui::AddPosition *ui;
    QSqlDatabase db;
};

#endif // ADDPOSITION_H
