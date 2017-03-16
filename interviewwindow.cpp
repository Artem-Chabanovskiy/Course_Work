#include "interviewwindow.h"
#include "ui_interviewwindow.h"
#include <QDialog>

interviewwindow::interviewwindow(QWidget *parent, QSqlDatabase db1) : QDialog(parent), ui(new Ui::interviewwindow) {
    db = db1;
    ui->setupUi(this);
    connect( ui->cansel_button, SIGNAL( clicked() ), SLOT( reject()  ) );
    QSqlQueryModel *model = new QSqlQueryModel(this);
    model->setQuery("SELECT name_of_position FROM position");
    ui->position_combobox->setModel(model);
}

interviewwindow::~interviewwindow()
{
    delete ui;
}
