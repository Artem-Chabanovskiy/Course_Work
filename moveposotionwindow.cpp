#include "moveposotionwindow.h"
#include "ui_moveposotionwindow.h"
#include <QDialog>

moveposotionwindow::moveposotionwindow(QWidget *parent, QSqlDatabase db1) :QDialog(parent), ui(new Ui::moveposotionwindow) {
    ui->setupUi(this);
    db = db1;
    connect( ui->cansel_button, SIGNAL( clicked() ), SLOT( reject()  ) );
}

moveposotionwindow::~moveposotionwindow()
{
    delete ui;
}
