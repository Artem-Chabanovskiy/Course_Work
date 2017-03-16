#include "moveposotionwindow.h"
#include "ui_moveposotionwindow.h"

moveposotionwindow::moveposotionwindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::moveposotionwindow)
{
    ui->setupUi(this);
}

moveposotionwindow::~moveposotionwindow()
{
    delete ui;
}
