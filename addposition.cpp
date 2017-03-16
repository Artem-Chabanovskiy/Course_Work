#include "addposition.h"
#include "ui_addposition.h"

AddPosition::AddPosition(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AddPosition)
{
    ui->setupUi(this);
}

AddPosition::~AddPosition()
{
    delete ui;
}
