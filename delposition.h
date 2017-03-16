#ifndef DELPOSITION_H
#define DELPOSITION_H

#include <QWidget>

namespace Ui {
class DelPosition;
}

class DelPosition : public QWidget
{
    Q_OBJECT

public:
    explicit DelPosition(QWidget *parent = 0);
    ~DelPosition();

private:
    Ui::DelPosition *ui;
};

#endif // DELPOSITION_H
