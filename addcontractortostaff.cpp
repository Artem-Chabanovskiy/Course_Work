#include "addcontractortostaff.h"
#include "ui_addcontractortostaff.h"

AddContractorToStaff::AddContractorToStaff(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AddContractorToStaff)
{
    ui->setupUi(this);
}

AddContractorToStaff::~AddContractorToStaff()
{
    delete ui;
}
