#include "interviewwindow.h"
#include "ui_interviewwindow.h"
#include <QDialog>
#include <QTabWidget>
#include <QAbstractItemView>

interviewwindow::interviewwindow(QWidget *parent, QSqlDatabase db1) : QDialog(parent), ui(new Ui::interviewwindow) {
    db = db1;
    ui->setupUi(this);
    connect (ui->add_interview_button, SIGNAL ( clicked() ), SLOT ( doInterview()));
    connect( ui->cansel_button, SIGNAL( clicked() ), SLOT( reject()  ) );
   // connect(ui->position_combobox, SIGNAL(currentIndexChanged (QString), this, SLOT(comboSelectItem(QString)));


   /* QSqlQueryModel *model = new QSqlQueryModel(this);
    model->setQuery("SELECT name_of_position, id_position FROM position WHERE id_position IN"
                    "(SELECT id_position FROM staffing_table WHERE total_membership < staff_complement);");
    ui->position_combobox->setModel(model);*/

    QString query;
    query = "SELECT id_contractor, surname, name FROM physical_person;";
    fillTable(ui->contr_to_int_table, query);
    ui->contr_to_int_table->setSelectionBehavior(QAbstractItemView::SelectRows);

    QString query_pos;
    query_pos = "SELECT id_position as idp, name_of_position as idp FROM position WHERE id_position IN "
            "(SELECT id_position FROM staffing_table WHERE total_membership < staff_complement);";
    qDebug() << query_pos;
    fillTable(ui->avaliable_position_table, query_pos);
    ui->contr_to_int_table->setSelectionBehavior(QAbstractItemView::SelectRows);
}

interviewwindow::~interviewwindow()
{
    delete ui;
}

int interviewwindow::doInterview() {
    QString id_contr, id_posit;
    int i = ui->contr_to_int_table->currentRow(); //строка contr
    int j = ui->avaliable_position_table->currentRow(); //stroka dolzhnosti
    id_contr = ui->contr_to_int_table->item(i,0)->text();
    id_posit = ui->avaliable_position_table->item(j,0)->text();
    //QString posit_to_int = ui->position_combobox->currentText();
    qDebug() << id_posit;
    qDebug() << id_contr;
    QString query_ins;
    query_ins = QString("INSERT INTO interview (id_position, id_contractor) "
                    "VALUES ('%1', '%2') ;").arg(id_posit, id_contr);
    db.exec(query_ins);
    qDebug() << db.lastError();
    interviewwindow::close();
    return 0;
}

int interviewwindow::fillTable(QTableWidget *tab, QString query){

    QSqlQuery sq = db.exec(query);
//    qDebug() << sq.lastError();
    int nc = tab->columnCount();
    tab->setRowCount(sq.size());
    sq.first();
    int row = 0;
    do{

        for (int i = 0; i < nc; i++){
            QTableWidgetItem* item = new QTableWidgetItem();
            item->setText(sq.value(i).toString());
            tab->setItem(row, i, item);
        }
        row++;
    }
    while (sq.next());
    tab->resizeColumnsToContents();
    return 0;
}
