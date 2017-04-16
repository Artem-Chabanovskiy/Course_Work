#include "firecontractorfromstaff.h"
#include "ui_firecontractorfromstaff.h"

FireContractorFromStaff::FireContractorFromStaff(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FireContractorFromStaff)
{
    ui->setupUi(this);
}

FireContractorFromStaff::~FireContractorFromStaff()
{
    delete ui;
}
