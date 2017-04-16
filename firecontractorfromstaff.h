#ifndef FIRECONTRACTORFROMSTAFF_H
#define FIRECONTRACTORFROMSTAFF_H

#include <QWidget>

namespace Ui {
class FireContractorFromStaff;
}

class FireContractorFromStaff : public QWidget
{
    Q_OBJECT

public:
    explicit FireContractorFromStaff(QWidget *parent = 0);
    ~FireContractorFromStaff();

private:
    Ui::FireContractorFromStaff *ui;
};

#endif // FIRECONTRACTORFROMSTAFF_H
