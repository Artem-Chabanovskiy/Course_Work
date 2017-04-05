#include "addcontractortostaff.h"
#include "ui_addcontractortostaff.h"
#include <QDialog>

AddContractorToStaff::AddContractorToStaff(QWidget *parent, QSqlDatabase db1) : QDialog (parent), ui(new Ui::AddContractorToStaff) {
    ui->setupUi(this);
    db = db1;
    connect( ui->cansel_bt, SIGNAL( clicked() ), SLOT( reject()  ) );
    connect(ui->add_to_staff_bt, SIGNAL (clicked()), SLOT( pushContrToStaff()));

    QString query;
    query = "SELECT f.surname, f.name, f.id_contractor, p.name_of_position FROM physical_person AS f, position AS p, interview AS i "
            "WHERE i.id_contractor = f.id_contractor AND p.id_position = i.id_position AND i.result IS NOT TRUE;";
    qDebug() << query;
    fillTable(ui->contr_to_staff_table, query);
    ui->contr_to_staff_table->setSelectionBehavior(QAbstractItemView::SelectRows);

    ui->contr_to_staff_table->setColumnHidden(2, true);
}

AddContractorToStaff::~AddContractorToStaff() {
    delete ui;
}

int AddContractorToStaff::fillTable(QTableWidget *tab, QString query){

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

int AddContractorToStaff::pushContrToStaff(){
    QString query, query_int_true;
    QString ident_code, position;
    int i = ui->contr_to_staff_table->currentRow(); //строка contr
    //surname = ui->contr_to_staff_table->item(i,0)->text();
    //name = ui->contr_to_staff_table->item(i,1)->text();
    ident_code = ui->contr_to_staff_table->item(i,2)->text();
    position = ui->contr_to_staff_table->item(i,3)->text();
    query = QString("INSERT INTO staff (id_contractor, id_position) "
                    "SELECT ph.id_contractor, p.id_position FROM physical_person AS ph, position AS p "
                    " WHERE ph.identificational_code = '%1' AND p.name_of_position = '%2';").arg(ident_code, position);
    query_int_true = QString("UPDATE interview "
                             "SET result = 'true' "
                             "WHERE id_contractor IN (SELECT id_contractor FROM physical_person "
                             "WHERE identificational_code = '%1');").arg(ident_code);
    qDebug() << query_int_true;
    db.exec(query);
    db.exec(query_int_true);
    qDebug() << db.lastError();
    AddContractorToStaff::close();
    return 0;
}

void AddContractorToStaff::on_addcontr_sort_bt_clicked()
{
    ui->contr_to_staff_table->sortItems(0);
}

void AddContractorToStaff::on_search_contr_to_st_bt_clicked()
{
    QString surname_search;
    QString query;
    surname_search = ui->search_contr_to_st_edit->text();
    query = QString("SELECT f.surname, f.name, f.id_contractor, p.name_of_position FROM physical_person AS f, position AS p, interview AS i "
                    "WHERE i.id_contractor = f.id_contractor "
                    "AND p.id_position = i.id_position AND i.result IS NOT TRUE AND f.surname = '%1';").arg(surname_search);
    QSqlQuery sq = db.exec(query);
    while (sq.next())
        slContr << sq.value(0).toString();
    if (!slContr.contains(surname_search)){
        ui->error_label->setText("<html><head/><body><p style=\"color:red;\">"
                              "Помилка ! <br>"
                              "Така людина не проходила <br> співбесіду!</p></body></html>");
    }
    else {
    fillTable(ui->contr_to_staff_table, query);
    ui->error_label->setText("");
    }
}

void AddContractorToStaff::on_refresh_bt_clicked()
{
    QString query;
    query = "SELECT f.surname, f.name, f.id_contractor, p.name_of_position FROM physical_person AS f, position AS p, interview AS i "
            "WHERE i.id_contractor = f.id_contractor AND p.id_position = i.id_position AND i.result IS NOT TRUE;";
    fillTable(ui->contr_to_staff_table, query);
}
