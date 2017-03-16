#ifndef INTERVIEWWINDOW_H
#define INTERVIEWWINDOW_H

#include <QWidget>

namespace Ui {
class interviewwindow;
}

class interviewwindow : public QWidget
{
    Q_OBJECT

public:
    explicit interviewwindow(QWidget *parent = 0);
    ~interviewwindow();

private:
    Ui::interviewwindow *ui;
};

#endif // INTERVIEWWINDOW_H
