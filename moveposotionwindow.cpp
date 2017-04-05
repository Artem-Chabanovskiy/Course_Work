#include "moveposotionwindow.h"
#include "ui_moveposotionwindow.h"
#include <QDialog>

moveposotionwindow::moveposotionwindow(QWidget *parent, QSqlDatabase db1) :QDialog(parent), ui(new Ui::moveposotionwindow) {
    ui->setupUi(this);
    db = db1;
    connect( ui->cansel_button, SIGNAL( clicked() ), SLOT( reject()  ) );
    connect( ui->move_button,   SIGNAL( clicked() ), SLOT( moveStaff()));

    QString query;
    query = "SELECT ph.surname, ph.name, ph.identificational_code, p.name_of_position "
            "FROM physical_person AS ph, position AS p, staff AS st "
            "WHERE ph.id_contractor = st.id_contractor AND p.id_position = st.id_position;";
    qDebug() << query;
    fillTable(ui->contr_move_po_tb, query);
    ui->contr_move_po_tb->setSelectionBehavior(QAbstractItemView::SelectRows);


     QSqlQueryModel *model = new QSqlQueryModel(this);
     model->setQuery("SELECT name_of_position FROM position WHERE id_position IN"
                     "(SELECT id_position FROM staffing_table WHERE total_membership < staff_complement);");
     ui->position_cb->setModel(model);
}

moveposotionwindow::~moveposotionwindow()
{
    delete ui;
}

int moveposotionwindow::moveStaff(){
    QString query;
    //QString functionCall = "position_move_function";
    int i = ui->contr_move_po_tb->currentRow();
    QString ident_code = ui->contr_move_po_tb->item(i,2)->text();
    QString position_from_move = ui->contr_move_po_tb->item(i,3)->text();
    QString position_to_move = ui->position_cb->currentText();
    qDebug() << ident_code;
    qDebug() << position_from_move;
    qDebug() << position_to_move;
    query = QString("SELECT \"position_move_function\"('%1', '%2', '%3')").arg(position_from_move, position_to_move, ident_code);
    db.exec(query);
    qDebug() << query;
    qDebug() << db.lastError();
    //query = QString("")
    moveposotionwindow::close();
    return 0;
}

int moveposotionwindow::fillTable(QTableWidget *tab, QString query){

    QSqlQuery sq = db.exec(query);
    qDebug() << sq.lastError();
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

void moveposotionwindow::on_position_move_sort_bt_clicked()
{
    ui->contr_move_po_tb->sortItems(0);
}

void moveposotionwindow::on_search_contr_to_st_bt_clicked()
{
    QString surname_search;
    QString query;
    surname_search = ui->search_contr_to_st_edit->text();
    query = QString("SELECT ph.surname, ph.name, ph.identificational_code, p.name_of_position "
                    "FROM physical_person AS ph, position AS p, staff AS st "
                    "WHERE ph.id_contractor = st.id_contractor AND p.id_position = st.id_position AND ph.surname = '%1' ;").arg(surname_search);
    QSqlQuery sq = db.exec(query);
    while (sq.next())
        slContr << sq.value(0).toString();
    if (!slContr.contains(surname_search)){
        ui->error_label->setText("<html><head/><body><p style=\"color:red;\">"
                              "Помилка ! <br>"
                              "Такої людини намає  <br> в базі!</p></body></html>");
    }
    else {
    fillTable(ui->contr_move_po_tb, query);
    ui->error_label->setText("");
    }
}

void moveposotionwindow::on_refresh_bt_clicked()
{
    QString query;
    query = "SELECT ph.surname, ph.name, ph.identificational_code, p.name_of_position "
            "FROM physical_person AS ph, position AS p, staff AS st "
            "WHERE ph.id_contractor = st.id_contractor AND p.id_position = st.id_position;";
    qDebug() << query;
    fillTable(ui->contr_move_po_tb, query);
}
