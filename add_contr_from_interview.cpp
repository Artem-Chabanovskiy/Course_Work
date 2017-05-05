#include "add_contr_from_interview.h"
#include "ui_add_contr_from_interview.h"

add_contr_from_interview::add_contr_from_interview(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::add_contr_from_interview)
{
    ui->setupUi(this);
}

add_contr_from_interview::~add_contr_from_interview()
{
    delete ui;
}
