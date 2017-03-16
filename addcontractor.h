#ifndef ADDCONTRACTOR_H
#define ADDCONTRACTOR_H

#include <QWidget>

namespace Ui {
class addcontractor;
}

class addcontractor : public QWidget
{
    Q_OBJECT

public:
    explicit addcontractor(QWidget *parent = 0);
    ~addcontractor();

private:
    Ui::addcontractor *ui;
};

#endif // ADDCONTRACTOR_H
