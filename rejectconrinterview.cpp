#include "rejectconrinterview.h"
#include "ui_rejectconrinterview.h"
#include <QDialog>
#include <QItemSelectionModel>
#include <string>


RejectConrInterview::RejectConrInterview(QWidget *parent, QSqlDatabase db1) :QDialog(parent), ui(new Ui::RejectConrInterview) {
    ui->setupUi(this);
    db = db1;

    //connecting buttons to slots
    connect( ui->cansel_bt, SIGNAL( clicked() ), SLOT( reject()  ) );
    connect( ui->contr_reject_bt,   SIGNAL( clicked() ), SLOT( ContrRejectInt()));

    //validators
    ui->search_contr_to_st_edit->setValidator(new QRegExpValidator(QRegExp("[A-Za-zА-Яа-яі]+"), this));

    //stretching table
    for (int i = 0; i < ui->contr_reject_tb->horizontalHeader()->count(); ++i)
    ui->contr_reject_tb->horizontalHeader()->setSectionResizeMode(i, QHeaderView::Stretch);

    //filling table
    QString query;
    query = "SELECT a.surname, a.name, p.name_of_position, i.id_interview "
            "FROM physical_person a, position p, interview i "
            "WHERE a.id_contractor = i.id_contractor AND p.id_position = i.id_position AND i.result = 'Розглядається';";
    qDebug() << query;
    fillTable(ui->contr_reject_tb, query);

    //configuring table
    ui->contr_reject_tb->setColumnHidden(3, true);
    ui->contr_reject_tb->setSelectionBehavior(QAbstractItemView::SelectRows);
}

RejectConrInterview::~RejectConrInterview() {
    delete ui;
}

int RejectConrInterview::ContrRejectInt() {

    QString query;

    QItemSelectionModel *selectionModel = ui->contr_reject_tb->selectionModel();
    QModelIndexList selectedRows = selectionModel->selectedRows();

    //check, if there is one selectes row
    if (selectedRows.size() != 1) {
        ui->error_label->setText("<html><head/><body><p style=\"color:red;\">"
                              "Error ! <br>"
                              "Choose one person!</p></body></html>");
    }
    else {
        int i = ui->contr_reject_tb->currentRow();
        QString inter_id = ui->contr_reject_tb->item(i,3)->text();
        query = QString("UPDATE interview "
                        "SET result = 'Відхилено' "
                        "WHERE id_interview = '%1' ;").arg(inter_id);
        db.exec(query);
        RejectConrInterview::close();
    }
    return 0;

}


int RejectConrInterview::fillTable(QTableWidget *tab, QString query){

    QSqlQuery sq = db.exec(query);
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

//searching by surname
void RejectConrInterview::on_search_contr_to_st_bt_clicked() {
    QStringList slContr;
    QString surname_search;
    QString query;

    surname_search = ui->search_contr_to_st_edit->text();
    query = QString("SELECT a.surname, a.name, p.name_of_position, i.id_interview "
                    "FROM physical_person a, position p, interview i "
                    "WHERE a.id_contractor = i.id_contractor AND p.id_position = i.id_position AND i.result = 'Розглядається' AND ph.surname ILIKE '%%1%';").arg(surname_search);

    QSqlQuery sq = db.exec(query);
    while (sq.next())
        slContr << sq.value(0).toString();

     //if query returns null
    if (slContr.empty()){
        ui->error_label->setText("<html><head/><body><p style=\"color:red;\">"
                              "Помилка ! <br>"
                              "Пошук не дав результатів!</p></body></html>");
    }
    else {
    fillTable(ui->contr_reject_tb, query);
    ui->error_label->setText("");
    }
}

//refreshing table
void RejectConrInterview::on_refresh_bt_clicked() {
    QString query;
    query = "SELECT a.surname, a.name, p.name_of_position, i.id_interview "
            "FROM physical_person a, position p, interview i "
            "WHERE a.id_contractor = i.id_contractor AND p.id_position = i.id_position AND i.result = 'Розглядається';";
    qDebug() << query;
    fillTable(ui->contr_reject_tb, query);
}

//making table transparent
void RejectConrInterview::paintEvent(QPaintEvent *) {
    ui->contr_reject_tb->setStyleSheet("background-color: transparent; border : 0 ");
}
