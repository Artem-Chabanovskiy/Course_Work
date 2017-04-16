#include "addcontractortostaff.h"
#include "ui_addcontractortostaff.h"
#include <QDialog>
#include <QItemSelectionModel>

AddContractorToStaff::AddContractorToStaff(QWidget *parent, QSqlDatabase db1) : QDialog (parent), ui(new Ui::AddContractorToStaff) {
    ui->setupUi(this);
    db = db1;
    connect( ui->cansel_bt, SIGNAL( clicked() ), SLOT( reject()  ) );
    connect(ui->add_to_staff_last_bt, SIGNAL (clicked()), SLOT( pushContrToStaff()));

    QString query;
    query = "SELECT f.surname, f.name, f.id_contractor, p.name_of_position, p.id_position FROM physical_person AS f, position AS p, interview AS i "
            "WHERE i.id_contractor = f.id_contractor AND p.id_position = i.id_position AND i.result = 'Розглядається' "
            "AND p.id_position IN (SELECT id_position FROM staffing_table WHERE total_membership < staff_complement) "
            "AND f.id_contractor NOT IN (SELECT st.id_contractor FROM staff st "
            "WHERE st.id_staff NOT IN (SELECT id_staff FROM cadre_on_position WHERE date_of_leaving_from_position IS NULL));";
    qDebug() << query;
    fillTable(ui->contr_to_staff_table, query);
    ui->contr_to_staff_table->setSelectionBehavior(QAbstractItemView::SelectRows);

    ui->contr_to_staff_table->setColumnHidden(2, true);
    ui->contr_to_staff_table->setColumnHidden(4, true);
    ui->salary_edit->setValidator(new QRegExpValidator(QRegExp("[0-9]+"), this));

    ui->add_to_staff_last_bt->setVisible(false);
    ui->min_info_lb->setVisible(false);
    ui->max_info_lb->setVisible(false);
    ui->salary_edit->setVisible(false);
    ui->inp_info_lb->setVisible(false);
    ui->min_sal_lb->setVisible(false);
    ui->max_sal_lb->setVisible(false);
    ui->cont_to_staff_lb_1->setVisible(false);
    ui->cont_to_staff_lb_2->setVisible(false);
    ui->cont_to_staff_sur_lb->setVisible(false);
    ui->cont_to_staff_name_lb->setVisible(false);
    ui->staff_posit_lb->setVisible(false);
    ui->return_bt->setVisible(false);
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
    //QString query, query_int_true;
    QString id_contr, id_position;

    int i = ui->contr_to_staff_table->currentRow(); //строка contr
    id_contr = ui->contr_to_staff_table->item(i,2)->text();
    id_position = ui->contr_to_staff_table->item(i,4)->text();

    int max_sal = ui->max_sal_lb->text().toInt();
    int min_sal = ui->min_sal_lb->text().toInt();
    int staff_salary = ui->salary_edit->text().toInt();
    QString staff_salary_str = ui->salary_edit->text();

    if (staff_salary <= max_sal && staff_salary >= min_sal) {
        QString query_function;
        query_function = QString("SELECT ""add_contractor_to_staff""('%1', '%2', '%3');")
                .arg(id_contr, id_position, staff_salary_str);
        qDebug() << query_function;
        QSqlQuery*  query = new QSqlQuery;
        query->exec(query_function);
        qDebug() << query->lastError();
        AddContractorToStaff::close();
        return 0;
    } else {
        ui->error_lb_2->setText("<html><head/><body><p style=\"color:red;\">"
                                                               "Помилка !"
                                                               "Некорректна зарплата!</p></body></html>");
    }


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
                    "AND p.id_position = i.id_position AND i.result ='Розглядається' "
                    "AND p.id_position IN (SELECT id_position FROM staffing_table WHERE total_membership < staff_complement) "
                    "AND f.surname = '%1';").arg(surname_search);
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
    query = "SELECT f.surname, f.name, f.id_contractor, p.name_of_position, p.id_position FROM physical_person AS f, position AS p, interview AS i "
            "WHERE i.id_contractor = f.id_contractor AND p.id_position = i.id_position AND i.result = 'Розглядається' "
            "AND p.id_position IN (SELECT id_position FROM staffing_table WHERE total_membership < staff_complement);";
    fillTable(ui->contr_to_staff_table, query);
}

void AddContractorToStaff::on_add_to_staff_first_bt_clicked()
{
    QItemSelectionModel *selectionModel = ui->contr_to_staff_table->selectionModel();
    QModelIndexList selectedRows = selectionModel->selectedRows();

    if (selectedRows.size() != 1) {
        ui->error_label->setText("<html><head/><body><p style=\"color:red;\">"
                              "Помилка ! <br>"
                              "Оберіть одного контрагента!</p></body></html>");
    }
    else  {
    ui->addcontr_sort_bt->setVisible(false);
    ui->add_to_staff_first_bt->setVisible(false);
    ui->contr_to_staff_table->setVisible(false);
    ui->refresh_bt->setVisible(false);
    ui->search_contr_to_st_bt->setVisible(false);
    ui->search_contr_to_st_edit->setVisible(false);
    ui->search_contr_to_st_lb->setVisible(false);
    ui->add_to_staff_first_bt->setVisible(false);
    ui->info_label->setVisible(false);

    ui->add_to_staff_last_bt->setVisible(true);
    ui->min_info_lb->setVisible(true);
    ui->max_info_lb->setVisible(true);
    ui->salary_edit->setVisible(true);
    ui->inp_info_lb->setVisible(true);
    ui->min_sal_lb->setVisible(true);
    ui->max_sal_lb->setVisible(true);
    ui->cont_to_staff_lb_1->setVisible(true);
    ui->cont_to_staff_lb_2->setVisible(true);
    ui->cont_to_staff_sur_lb->setVisible(true);
    ui->cont_to_staff_name_lb->setVisible(true);
    ui->staff_posit_lb->setVisible(true);
    ui->return_bt->setVisible(true);

    ui->error_label->setText("");

    int max_salary, min_salary;
    int i = ui->contr_to_staff_table->currentRow();

    QString name_position = ui->contr_to_staff_table->item(i,3)->text();
    QString contr_surname = ui->contr_to_staff_table->item(i,0)->text();
    QString contr_name = ui->contr_to_staff_table->item(i,1)->text();
    ui->cont_to_staff_sur_lb->setText(contr_surname);
    ui->cont_to_staff_name_lb->setText(contr_name);
    ui->staff_posit_lb->setText(name_position);

    QString id_position = ui->contr_to_staff_table->item(i,4)->text();
    QString query_function_min, query_function_max;

    query_function_min = QString("SELECT ""get_min_salary('%1')"";").arg(id_position);
    QSqlQuery*  query_min = new QSqlQuery;
    query_min->exec(query_function_min);
    query_min->next();
    min_salary = query_min->value(0).toInt();
    query_function_max = QString("SELECT ""get_max_salary('%1')"";").arg(id_position);
    QSqlQuery*  query_max = new QSqlQuery;
    query_max->exec(query_function_max);
    query_max->next();
    max_salary = query_max->value(0).toInt();
    ui->min_sal_lb->setText(QString::number(min_salary));
    ui->max_sal_lb->setText(QString::number(max_salary));

    }

}

void AddContractorToStaff::on_return_bt_clicked()
{
    ui->addcontr_sort_bt->setVisible(true);
    ui->add_to_staff_first_bt->setVisible(true);
    ui->contr_to_staff_table->setVisible(true);
    ui->refresh_bt->setVisible(true);
    ui->search_contr_to_st_bt->setVisible(true);
    ui->search_contr_to_st_edit->setVisible(true);
    ui->search_contr_to_st_lb->setVisible(true);
    ui->add_to_staff_first_bt->setVisible(true);
     ui->info_label->setVisible(true);

    ui->add_to_staff_last_bt->setVisible(false);
    ui->min_info_lb->setVisible(false);
    ui->max_info_lb->setVisible(false);
    ui->salary_edit->setVisible(false);
    ui->inp_info_lb->setVisible(false);
    ui->min_sal_lb->setVisible(false);
    ui->max_sal_lb->setVisible(false);
    ui->cont_to_staff_lb_1->setVisible(false);
    ui->cont_to_staff_lb_2->setVisible(false);
    ui->cont_to_staff_sur_lb->setVisible(false);
    ui->cont_to_staff_name_lb->setVisible(false);
    ui->staff_posit_lb->setVisible(false);
    ui->return_bt->setVisible(false);

}
