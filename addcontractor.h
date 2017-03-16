#ifndef ADDCONTRACTOR_H
#define ADDCONTRACTOR_H

#include <QWidget>
#include <QDialog>
#include <QtSql>
#include <QCompleter>

namespace Ui {
class addcontractor;
}

class addcontractor : public QDialog
{
    Q_OBJECT

public:
    explicit addcontractor(QWidget *parent = 0, QSqlDatabase db1 = QSqlDatabase());
    ~addcontractor();

private slots:
    int ContractorInsert();

private:
    Ui::addcontractor *ui;
    QSqlDatabase db;

};

#endif // ADDCONTRACTOR_H
