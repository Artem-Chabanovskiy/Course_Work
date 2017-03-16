#include "delposition.h"
#include "ui_delposition.h"

DelPosition::DelPosition(QWidget *parent, QSqlDatabase db1) :
    QDialog(parent),
    ui(new Ui::DelPosition)
{
    ui->setupUi(this);
    db = db1;
    connect( ui->cansel_bt, SIGNAL( clicked() ), SLOT( reject()  ) );
}

DelPosition::~DelPosition()
{
    delete ui;
}
