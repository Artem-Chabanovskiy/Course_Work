#ifndef ADDPOSITION_H
#define ADDPOSITION_H

#include <QWidget>

namespace Ui {
class AddPosition;
}

class AddPosition : public QWidget
{
    Q_OBJECT

public:
    explicit AddPosition(QWidget *parent = 0);
    ~AddPosition();

private:
    Ui::AddPosition *ui;
};

#endif // ADDPOSITION_H
