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


   QSqlQueryModel *model = new QSqlQueryModel(this);
    model->setQuery("SELECT name_of_position, id_position FROM position WHERE id_position IN"
                    "(SELECT id_position FROM staffing_table WHERE total_membership < staff_complement);");
    ui->position_cb->setModel(model);

    QString query;
    query = "SELECT DISTINCT ph.surname, ph.name, ph.id_contractor FROM physical_person ph, interview i ";
            //"WHERE ph.id_contractor NOT IN (SELECT i.id_contractor FROM interview i);";
    fillTable(ui->contr_to_int_table, query);
    ui->contr_to_int_table->setSelectionBehavior(QAbstractItemView::SelectRows);

    ui->contr_to_int_table->setColumnHidden(2, true);

    /*QString query_pos;
    query_pos = "SELECT name_of_position FROM position WHERE id_position IN "
            "(SELECT id_position FROM staffing_table WHERE total_membership < staff_complement);";
    qDebug() << query_pos;
    fillTable(ui->avaliable_position_table, query_pos);
    ui->avaliable_position_table->setSelectionBehavior(QAbstractItemView::SelectRows);*/
}

interviewwindow::~interviewwindow()
{
    delete ui;
}

int interviewwindow::doInterview() {
    QString id_contr;
    int i = ui->contr_to_int_table->currentRow(); //строка contr
    //int j = ui->avaliable_position_table->currentRow(); //stroka dolzhnosti
    id_contr = ui->contr_to_int_table->item(i,2)->text();
    //id_posit = ui->avaliable_position_table->item(j,0)->text();
    QString posit_to_inter = ui->position_cb->currentText();
    qDebug() << id_contr;
    qDebug() << posit_to_inter;
    //QString query_ins;
    QSqlQuery*  query = new QSqlQuery;
    QString query_id_pos = QString("SELECT id_position FROM position WHERE name_of_position = '%1';").arg(posit_to_inter);
    query->exec(query_id_pos);
    query->next();
    QString id_posit = query->value(0).toString();
    QString query_function = QString("SELECT ""interview_do""('%1', '%2')").arg(id_contr, id_posit);
    //db.exec(query);
    QSqlQuery*  query_ins = new QSqlQuery;
    query_ins->exec(query_function);
    query_ins->next();
    int func_result = query_ins->value(0).toInt();
    qDebug() << db.lastError();
    if (func_result == 0) {
        ui->error_label->setText("<html><head/><body><p style=\"color:red;\">"
                                 "Помилка ! <br>"
                                 "Ця особа вже проходила <br> співбесіду на цю посаду!</p></body></html>");
    }
    else  interviewwindow::close();
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

void interviewwindow::on_interview_sort_bt_clicked()
{
    ui->contr_to_int_table->sortItems(0);
}

void interviewwindow::on_search_contr_to_st_bt_clicked()
{
    QString surname_search;
    QString query;
    surname_search = ui->search_contr_to_st_edit->text();
    query = QString("SELECT DISTINCT ph.surname, ph.name, ph.id_contractor FROM physical_person ph, interview i "
                    "WHERE ph.surname = '%1' ; ").arg(surname_search);
    QSqlQuery sq = db.exec(query);
    while (sq.next())
        slContr << sq.value(0).toString();
    if (!slContr.contains(surname_search)){
        ui->error_label->setText("<html><head/><body><p style=\"color:red;\">"
                              "Помилка ! <br>"
                              "Такої людини намає  <br> в базі!</p></body></html>");
    }
    else {
    fillTable(ui->contr_to_int_table, query);
    ui->error_label->setText("");
    }
}

void interviewwindow::on_refresh_bt_clicked()
{
    QString query;
    query = "SELECT DISTINCT ph.surname, ph.name, ph.id_contractor FROM physical_person ph, interview i ;";
            //"WHERE ph.id_contractor NOT IN (SELECT i.id_contractor FROM interview i);";
    fillTable(ui->contr_to_int_table, query);
}
