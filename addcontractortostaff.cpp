#include "addcontractortostaff.h"
#include "ui_addcontractortostaff.h"
#include <QDialog>

AddContractorToStaff::AddContractorToStaff(QWidget *parent, QSqlDatabase db1) : QDialog (parent), ui(new Ui::AddContractorToStaff) {
    ui->setupUi(this);
    db = db1;
    connect( ui->cansel_bt, SIGNAL( clicked() ), SLOT( reject()  ) );
}

AddContractorToStaff::~AddContractorToStaff() {
    delete ui;
}
