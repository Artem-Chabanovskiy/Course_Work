#include "delposition.h"
#include "ui_delposition.h"

DelPosition::DelPosition(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DelPosition)
{
    ui->setupUi(this);
}

DelPosition::~DelPosition()
{
    delete ui;
}
