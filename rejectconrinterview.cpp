#include "rejectconrinterview.h"
#include "ui_rejectconrinterview.h"
#include <QDialog>
#include <QItemSelectionModel>
#include <string>


RejectConrInterview::RejectConrInterview(QWidget *parent, QSqlDatabase db1) :QDialog(parent), ui(new Ui::RejectConrInterview) {
    ui->setupUi(this);
    db = db1;
    connect( ui->cansel_bt, SIGNAL( clicked() ), SLOT( reject()  ) );
    connect( ui->contr_reject_bt,   SIGNAL( clicked() ), SLOT( ContrRejectInt()));

    QString query;
    query = "SELECT a.surname, a.name, p.name_of_position, i.id_interview "
            "FROM physical_person a, position p, interview i "
            "WHERE a.id_contractor = i.id_contractor AND p.id_position = i.id_position AND i.result = 'Розглядається';";
    qDebug() << query;
    fillTable(ui->contr_reject_tb, query);

    ui->contr_reject_tb->setColumnHidden(3, true);
    ui->contr_reject_tb->setSelectionBehavior(QAbstractItemView::SelectRows);
}

RejectConrInterview::~RejectConrInterview()
{
    delete ui;
}

int RejectConrInterview::ContrRejectInt() {
    QString query;
    QItemSelectionModel *selectionModel = ui->contr_reject_tb->selectionModel();
    QModelIndexList selectedRows = selectionModel->selectedRows();

    if (selectedRows.size() != 1) {
        ui->error_label->setText("<html><head/><body><p style=\"color:red;\">"
                              "Помилка ! <br>"
                              "Оберіть одного контрагента!</p></body></html>");
    }
    else {
        int i = ui->contr_reject_tb->currentRow();
        QString inter_id = ui->contr_reject_tb->item(i,3)->text();
        query = QString("UPDATE interview "
                        "SET result = 'Відхилено' "
                        "WHERE id_interview = '%1' ;").arg(inter_id);
        qDebug() << query;
        db.exec(query);
        qDebug() << db.lastError();
        RejectConrInterview::close();
    }
    return 0;

}

int RejectConrInterview::fillTable(QTableWidget *tab, QString query){

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
