#include "addposition.h"
#include "ui_addposition.h"
#include <QDialog>

AddPosition::AddPosition(QWidget *parent, QSqlDatabase db1) :
    QDialog(parent),
    ui(new Ui::AddPosition)
{
    ui->setupUi(this);
    db = db1;
    connect( ui->cansel_bt, SIGNAL( clicked() ), SLOT( reject()  ) );
    connect(ui->add_position_bt, SIGNAL ( clicked() ), SLOT( add_position()));
}

AddPosition::~AddPosition()
{
    delete ui;
}

int AddPosition::add_position(){
    QString query, query1, query2;
    QString name_position, payment, staff_compl;
    name_position = ui->position_name_edit->text();
    payment = ui->position_salary_edit->text();
    staff_compl = ui->staff_complement_edit->text();
    query = QString("INSERT INTO position (name_of_position) VALUES ('%1')").arg(name_position);
    db.exec(query);
    qDebug() << db.lastError();
    query1 = QString("UPDATE staffing_table"
                    " SET total_membership = '0', payment_of_position = '%1', staff_complement = '%2' ").arg(payment, staff_compl);
    query2 = QString("FROM position WHERE (id_staffing_table = currval('public.staffing_table_id_staffing_table_seq')) ;");
    query = query1 + query2;
    db.exec(query);
    qDebug() << db.lastError();
    AddPosition::close();
    return 0;
}
