#ifndef ADDCONTRACTORTOSTAFF_H
#define ADDCONTRACTORTOSTAFF_H

#include <QWidget>

namespace Ui {
class AddContractorToStaff;
}

class AddContractorToStaff : public QWidget
{
    Q_OBJECT

public:
    explicit AddContractorToStaff(QWidget *parent = 0);
    ~AddContractorToStaff();

private:
    Ui::AddContractorToStaff *ui;
};

#endif // ADDCONTRACTORTOSTAFF_H
