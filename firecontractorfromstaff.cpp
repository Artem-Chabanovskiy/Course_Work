#include "firecontractorfromstaff.h"
#include "ui_firecontractorfromstaff.h"

FireContractorFromStaff::FireContractorFromStaff(QWidget *parent, QSqlDatabase db1) : QDialog (parent), ui(new Ui::FireContractorFromStaff)
{
    ui->setupUi(this);
    db = db1;
    connect( ui->cansel_bt, SIGNAL( clicked() ), SLOT( reject()  ) );
    connect(ui->fire_bt, SIGNAL (clicked()), SLOT( fireContrFromStaff()));

    QString query;
    query = "SELECT st.id_staff, f.surname, f.name, f.id_contractor, p.name_of_position, p.id_position FROM physical_person AS f, position AS p, staff AS st "
            "WHERE st.id_contractor = f.id_contractor AND p.id_position = st.id_position "
            "AND st.id_staff IN (SELECT id_staff FROM cadre_on_position WHERE date_of_leaving_from_position IS NULL);";
    qDebug() << query;
    fillTable(ui->contr_from_staff_tb, query);
    ui->contr_from_staff_tb->setSelectionBehavior(QAbstractItemView::SelectRows);

    ui->contr_from_staff_tb->setColumnHidden(0, true);
    ui->contr_from_staff_tb->setColumnHidden(3, true);
    ui->contr_from_staff_tb->setColumnHidden(5, true);
}

FireContractorFromStaff::~FireContractorFromStaff()
{
    delete ui;
}

int FireContractorFromStaff::fireContrFromStaff() {
    QString id_contr, id_position, id_staff;
    QString query;
    QItemSelectionModel *selectionModel = ui->contr_from_staff_tb->selectionModel();
    QModelIndexList selectedRows = selectionModel->selectedRows();
    if (selectedRows.size() != 1) {
        ui->error_lb->setText("<html><head/><body><p style=\"color:red;\">"
                              "Помилка ! <br>"
                              "Оберіть одного контрагента!</p></body></html>");
    }
    else {
        int i = ui->contr_from_staff_tb->currentRow(); //строка contr
        id_staff = ui->contr_from_staff_tb->item(i,0)->text();
        //id_contr = ui->contr_from_staff_tb->item(i,2)->text();
        id_position = ui->contr_from_staff_tb->item(i,5)->text();
        query = QString("SELECT ""fire_contractor_from_staff""('%1', '%2')").arg(id_staff, id_position);
        db.exec(query);
        qDebug() << query;
        qDebug() << db.lastError();
        //query = QString("")
        FireContractorFromStaff::close();
    }
    return 0;
}

int FireContractorFromStaff::fillTable(QTableWidget *tab, QString query){

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
void FireContractorFromStaff::on_cansel_bt_2_clicked()
{
    ui->contr_from_staff_tb->sortItems(0);
}

void FireContractorFromStaff::on_search_contr_from_st_bt_clicked()
{

}
