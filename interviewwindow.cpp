#include "interviewwindow.h"
#include "ui_interviewwindow.h"

interviewwindow::interviewwindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::interviewwindow)
{
    ui->setupUi(this);
}

interviewwindow::~interviewwindow()
{
    delete ui;
}
