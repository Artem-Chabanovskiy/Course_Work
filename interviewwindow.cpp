#include "interviewwindow.h"
#include "ui_interviewwindow.h"
#include <QDialog>
#include <QTabWidget>
#include <QAbstractItemView>
#include <QItemSelectionModel>
#include <string>

interviewwindow::interviewwindow(QWidget *parent, QSqlDatabase db1) : QDialog(parent), ui(new Ui::interviewwindow) {
    db = db1;
    ui->setupUi(this);
    connect (ui->add_interview_button, SIGNAL ( clicked() ), SLOT ( doInterview()));
    connect( ui->cansel_button, SIGNAL( clicked() ), SLOT( reject()  ) );
   // connect(ui->position_combobox, SIGNAL(currentIndexChanged (QString), this, SLOT(comboSelectItem(QString)));

    ui->search_contr_to_st_edit->setValidator(new QRegExpValidator(QRegExp("[A-Za-zА-Яа-яі]+"), this));

    for (int i = 0; i < ui->contr_to_int_table->horizontalHeader()->count(); ++i)
    ui->contr_to_int_table->horizontalHeader()->setSectionResizeMode(i, QHeaderView::Stretch);

    QSqlQuery*  full_info_exist = new QSqlQuery;
    full_info_exist->exec("SELECT count(*) FROM  staff_position_full_info");
    full_info_exist->next();
    int check = full_info_exist->value(0).toInt();

    if (check == 0) {

        QSqlQueryModel *model = new QSqlQueryModel(this);
        model->setQuery("SELECT name_of_position, id_position FROM position;");
        ui->position_cb->setModel(model);
    }
    else {
        QSqlQueryModel *model = new QSqlQueryModel(this);
        model->setQuery("SELECT name_of_position, id_position FROM position WHERE id_position IN"
                       "(SELECT id_position FROM staff_position_full_info WHERE avaliable > 0);");
        ui->position_cb->setModel(model);
    }

    QString query;
    query = "SELECT ph.surname, ph.name, ph.id_contractor FROM physical_person ph "
            "WHERE ph.id_contractor NOT IN "
            "(SELECT st.id_contractor FROM staff st, cadre_on_position cp WHERE st.id_staff = cp.id_staff AND cp.date_of_leaving_from_position IS NULL);";
    fillTable(ui->contr_to_int_table, query);
    ui->contr_to_int_table->setSelectionBehavior(QAbstractItemView::SelectRows);

    ui->contr_to_int_table->setColumnHidden(2, true);
}

interviewwindow::~interviewwindow()
{
    delete ui;
}

int interviewwindow::doInterview() {
    QString id_contr;
    QItemSelectionModel *selectionModel = ui->contr_to_int_table->selectionModel();
    QModelIndexList selectedRows = selectionModel->selectedRows();
    if (selectedRows.size() != 1) {
        ui->error_label->setText("<html><head/><body><p style=\"color:red;\">"
                              "Помилка ! <br>"
                              "Оберіть одного контрагента!</p></body></html>");
    }
    else {
        int i = ui->contr_to_int_table->currentRow(); //строка contr
        id_contr = ui->contr_to_int_table->item(i,2)->text();
        QString posit_to_inter = ui->position_cb->currentText();

        QSqlQuery*  query = new QSqlQuery;
        QString query_id_pos = QString("SELECT id_position FROM position WHERE name_of_position = '%1';").arg(posit_to_inter);
        query->exec(query_id_pos);
        query->next();

        QString id_posit = query->value(0).toString();
        QString query_function = QString("SELECT ""interview_do""('%1', '%2');").arg(id_contr, id_posit);
        QSqlQuery*  query_ins = new QSqlQuery;
        query_ins->exec(query_function);

        if (query_ins->lastError().isValid()) {
         ui->error_label->setStyleSheet("QLabel { color : red; }");
         std::string s = query_ins->lastError().text().toUtf8().constData();;
            s = s.substr(0,s.find("(P0001)"));
            ui->error_label->setWordWrap(true);
            ui->error_label->setText(QString::fromUtf8(s.c_str()));
        }
        else {
            interviewwindow::close();
        }
    }
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
    QStringList slContr;
    QString surname_search;
    QString query;
    surname_search = ui->search_contr_to_st_edit->text();
    query = QString("SELECT ph.surname, ph.name, ph.id_contractor FROM physical_person ph "
                    "WHERE ph.id_contractor NOT IN "
                    "(SELECT st.id_contractor FROM staff st, cadre_on_position cp WHERE st.id_staff = cp.id_staff AND cp.date_of_leaving_from_position IS NULL) "
                    "AND ph.surname ILIKE '%%1%' ; ").arg(surname_search);
    QSqlQuery sq = db.exec(query);
    while (sq.next())
        slContr << sq.value(0).toString();
    if (slContr.empty()){
        ui->error_label->setText("<html><head/><body><p style=\"color:red;\">"
                              "Помилка ! <br>"
                              "Пошук не дав результатів!</p></body></html>");
    }
    else {
    fillTable(ui->contr_to_int_table, query);
    ui->error_label->setText("");
    }
}

void interviewwindow::on_refresh_bt_clicked()
{
    QString query;
    query = "SELECT ph.surname, ph.name, ph.id_contractor FROM physical_person ph "
            "WHERE ph.id_contractor NOT IN "
            "(SELECT st.id_contractor FROM staff st, cadre_on_position cp WHERE st.id_staff = cp.id_staff AND cp.date_of_leaving_from_position IS NULL);";
    fillTable(ui->contr_to_int_table, query);
}


void interviewwindow::paintEvent(QPaintEvent *) {

    ui->contr_to_int_table->setStyleSheet("background-color: transparent; border : 0 ");

}
