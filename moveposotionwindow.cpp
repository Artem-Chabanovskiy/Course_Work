#include "moveposotionwindow.h"
#include "ui_moveposotionwindow.h"
#include <QDialog>
#include <QItemSelectionModel>
#include <string>

moveposotionwindow::moveposotionwindow(QWidget *parent, QSqlDatabase db1) :QDialog(parent), ui(new Ui::moveposotionwindow) {
    ui->setupUi(this);
    db = db1;

    //connecting buttons to slots
    connect( ui->cansel_button, SIGNAL( clicked() ), SLOT( reject()  ) );
    connect( ui->move_button,   SIGNAL( clicked() ), SLOT( moveStaff()));
    connect( ui->position_cb, SIGNAL(currentTextChanged(QString)), SLOT( salary_input()));

    //validators
    ui->search_contr_to_st_edit->setValidator(new QRegExpValidator(QRegExp("[A-Za-zА-Яа-яі]+"), this));

    //stretching table
    for (int i = 0; i < ui->contr_move_po_tb->horizontalHeader()->count(); ++i)
    ui->contr_move_po_tb->horizontalHeader()->setSectionResizeMode(i, QHeaderView::Stretch);

    //filling table
    QString query;
    query = "SELECT ph.surname, ph.name, ph.id_contractor, p.name_of_position "
            "FROM physical_person AS ph, position AS p, staff AS st "
            "WHERE ph.id_contractor = st.id_contractor AND p.id_position = st.id_position "
            "AND st.id_staff IN (SELECT id_staff FROM cadre_on_position WHERE date_of_leaving_from_position IS NULL AND effective_date_of_the_position <= current_date);";
    fillTable(ui->contr_move_po_tb, query);

    //configuring table
    ui->contr_move_po_tb->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->contr_move_po_tb->setColumnHidden(2, true);

    //filling position combobox
    QSqlQueryModel *model = new QSqlQueryModel(this);
    model->setQuery("SELECT name_of_position FROM position WHERE id_position IN"
                    "(SELECT id_position FROM staff_position_full_info WHERE avaliable > 0);");
    ui->position_cb->setModel(model);
}

moveposotionwindow::~moveposotionwindow() {
    delete ui;
}

int moveposotionwindow::moveStaff(){

    QString query;
    QItemSelectionModel *selectionModel = ui->contr_move_po_tb->selectionModel();
    QModelIndexList selectedRows = selectionModel->selectedRows();

    //collecting info about salary
    int max_sal = ui->max_sal_lb->text().toInt();
    int min_sal = ui->min_sal_lb->text().toInt();
    int staff_salary = ui->salary_edit->text().toInt();
    QString staff_salary_str = ui->salary_edit->text();

     //check, if there is one selectes ro
    if (selectedRows.size() != 1) {
        ui->error_label->setText("<html><head/><body><p style=\"color:red;\">"
                                 "Error ! <br>"
                                 "Choose one person!</p></body></html>");
    }
    else
        //check, if the inputed salary ok
        if (staff_salary <= max_sal && staff_salary >= min_sal) {

            //collecting info about contractor and positions
            int i = ui->contr_move_po_tb->currentRow();
            QString ident_code = ui->contr_move_po_tb->item(i,2)->text();
            QString position_from_move = ui->contr_move_po_tb->item(i,3)->text();
            QString position_to_move = ui->position_cb->currentText();

            //creating query and exec. function
            query = QString("SELECT \"position_move_function\"('%1', '%2', '%3', '%4')").arg(position_from_move, position_to_move, ident_code, staff_salary_str);
            db.exec(query);

            moveposotionwindow::close();
        }
        else {
            ui->error_label->setText("<html><head/><body><p style=\"color:red;\">"
                                  "Error ! <br>"
                                  "Wrong salary!</p></body></html>");
        }
    return 0;
}

//selecting min and max salary, depending on position in combobox
int moveposotionwindow::salary_input() {

    //get id of position
    QString position_to_move = ui->position_cb->currentText();
    QString query;
    query = QString("SELECT id_position FROM position WHERE name_of_position = '%1';").arg(position_to_move);
    QSqlQuery*  query_id_posit = new QSqlQuery;
    query_id_posit->exec(query);
    query_id_posit->next();
    QString id_position_to_move = query_id_posit->value(0).toString();

    //getting min and max salary of position
    QString query_function_min, query_function_max;

    //min
    query_function_min = QString("SELECT ""get_min_salary('%1')"";").arg(id_position_to_move);
    QSqlQuery*  query_min = new QSqlQuery;
    query_min->exec(query_function_min);
    query_min->next();
    int min_salary = query_min->value(0).toInt();

    //max
    query_function_max = QString("SELECT ""get_max_salary('%1')"";").arg(id_position_to_move);
    QSqlQuery*  query_max = new QSqlQuery;
    query_max->exec(query_function_max);
    query_max->next();
    int max_salary = query_max->value(0).toInt();

    //setting min and max salary to labels
    ui->min_sal_lb->setText(QString::number(min_salary));
    ui->max_sal_lb->setText(QString::number(max_salary));

    return 0;
}

//filling table
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

//sorting by surname
void moveposotionwindow::on_position_move_sort_bt_clicked() {
    ui->contr_move_po_tb->sortItems(0);
}

//search by surname
void moveposotionwindow::on_search_contr_to_st_bt_clicked() {

    QStringList slContr;
    QString surname_search;
    QString query;

    surname_search = ui->search_contr_to_st_edit->text();
    query = QString("SELECT ph.surname, ph.name, ph.identificational_code, p.name_of_position "
                    "FROM physical_person AS ph, position AS p, staff AS st "
                    "WHERE ph.id_contractor = st.id_contractor AND p.id_position = st.id_position "
                    "AND st.id_staff IN (SELECT id_staff FROM cadre_on_position WHERE date_of_leaving_from_position IS NULL), AND ph.surname ILIKE '%%1%';").arg(surname_search);

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
    fillTable(ui->contr_move_po_tb, query);
    ui->error_label->setText("");
    }
}

//refreshing table
void moveposotionwindow::on_refresh_bt_clicked() {
    QString query;
    query = "SELECT ph.surname, ph.name, ph.identificational_code, p.name_of_position "
            "FROM physical_person AS ph, position AS p, staff AS st "
            "WHERE ph.id_contractor = st.id_contractor AND p.id_position = st.id_position "
            "AND st.id_staff IN (SELECT id_staff FROM cadre_on_position WHERE date_of_leaving_from_position IS NULL);";
    fillTable(ui->contr_move_po_tb, query);
}

//making table transparent
void moveposotionwindow::paintEvent(QPaintEvent *) {
    ui->contr_move_po_tb->setStyleSheet("background-color: transparent; border : 0 ");
}
