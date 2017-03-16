#ifndef MOVEPOSOTIONWINDOW_H
#define MOVEPOSOTIONWINDOW_H

#include <QWidget>

namespace Ui {
class moveposotionwindow;
}

class moveposotionwindow : public QWidget
{
    Q_OBJECT

public:
    explicit moveposotionwindow(QWidget *parent = 0);
    ~moveposotionwindow();

private:
    Ui::moveposotionwindow *ui;
};

#endif // MOVEPOSOTIONWINDOW_H
