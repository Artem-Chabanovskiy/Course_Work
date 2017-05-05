#ifndef ADD_CONTR_FROM_INTERVIEW_H
#define ADD_CONTR_FROM_INTERVIEW_H

#include <QWidget>
#include <QDialog>
#include <QtSql>
#include <QCompleter>
#include <QTabWidget>
#include <QTableWidget>



namespace Ui {
class add_contr_from_interview;
}

class add_contr_from_interview : public QWidget
{
    Q_OBJECT

public:
    explicit add_contr_from_interview(QWidget *parent = 0);
    ~add_contr_from_interview();

private:
    Ui::add_contr_from_interview *ui;
};

#endif // ADD_CONTR_FROM_INTERVIEW_H
