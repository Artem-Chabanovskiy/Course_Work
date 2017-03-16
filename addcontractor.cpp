#include "addcontractor.h"
#include "ui_addcontractor.h"

addcontractor::addcontractor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::addcontractor)
{
    ui->setupUi(this);
}

addcontractor::~addcontractor()
{
    delete ui;
}
