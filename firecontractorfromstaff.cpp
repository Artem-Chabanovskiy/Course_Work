#include "firecontractorfromstaff.h"
#include "ui_firecontractorfromstaff.h"

FireContractorFromStaff::FireContractorFromStaff(QWidget *parent, QSqlDatabase db1) : QDialog (parent), ui(new Ui::FireContractorFromStaff) {

    ui->setupUi(this);
    db = db1;

    //connecting buttons to slots
    connect( ui->cansel_bt, SIGNAL( clicked() ), SLOT( reject()  ) );
    connect(ui->fire_bt, SIGNAL (clicked()), SLOT( fireContrFromStaff()));

    //validators
    ui->search_contr_from_st_edit->setValidator(new QRegExpValidator(QRegExp("[A-Za-zА-Яа-яі]+"), this));

    //stretching table
    for (int i = 0; i < ui->contr_from_staff_tb->horizontalHeader()->count(); ++i)
    ui->contr_from_staff_tb->horizontalHeader()->setSectionResizeMode(i, QHeaderView::Stretch);

    //filling table
    QString query;
    query = "SELECT st.id_staff, f.surname, f.name, f.id_contractor, p.name_of_position, p.id_position FROM physical_person AS f, position AS p, staff AS st "
            "WHERE st.id_contractor = f.id_contractor AND p.id_position = st.id_position "
            "AND st.id_staff IN (SELECT id_staff FROM cadre_on_position WHERE date_of_leaving_from_position IS NULL AND effective_date_of_the_position <= current_date);";
    fillTable(ui->contr_from_staff_tb, query);

    //configuring table
    ui->contr_from_staff_tb->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->contr_from_staff_tb->setColumnHidden(0, true);
    ui->contr_from_staff_tb->setColumnHidden(3, true);
    ui->contr_from_staff_tb->setColumnHidden(5, true);
}

FireContractorFromStaff::~FireContractorFromStaff() {
    delete ui;
}

int FireContractorFromStaff::fireContrFromStaff() {

    QString id_position, id_staff;
    QString query;

    QItemSelectionModel *selectionModel = ui->contr_from_staff_tb->selectionModel();
    QModelIndexList selectedRows = selectionModel->selectedRows();

     //check, if there is one selectes row
    if (selectedRows.size() != 1) {
        ui->error_lb->setText("<html><head/><body><p style=\"color:red;\">"
                              "Error! <br>"
                              "Choose one person!</p></body></html>");
    }
    //selecting info and exec function
    else {
        int i = ui->contr_from_staff_tb->currentRow();
        id_staff = ui->contr_from_staff_tb->item(i,0)->text();
        id_position = ui->contr_from_staff_tb->item(i,5)->text();
        query = QString("SELECT ""fire_contractor_from_staff""('%1', '%2')").arg(id_staff, id_position);
        db.exec(query);
        FireContractorFromStaff::close();
    }
    return 0;
}

//filling table
int FireContractorFromStaff::fillTable(QTableWidget *tab, QString query){

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

//search person in table
void FireContractorFromStaff::on_search_contr_from_st_bt_clicked() {
    //creating and exec query
    QStringList slContr;
    QString surname_search;
    QString query;
    surname_search = ui->search_contr_from_st_edit->text();
    query = QString("SELECT st.id_staff, f.surname, f.name, f.id_contractor, p.name_of_position, p.id_position FROM physical_person AS f, position AS p, staff AS st "
                    "WHERE st.id_contractor = f.id_contractor AND p.id_position = st.id_position AND f.surname ILIKE '%%1%' "
                    "AND st.id_staff IN (SELECT id_staff FROM cadre_on_position WHERE date_of_leaving_from_position IS NULL);").arg(surname_search);
    QSqlQuery sq = db.exec(query);
    while (sq.next());
        slContr << sq.value(0).toString();
    //if query returns null
    if (slContr.empty()){
        ui->error_lb->setText("<html><head/><body><p style=\"color:red;\">"
                              "Error ! <br>"
                              "No people with such <br> characters in surname!</p></body></html>");
    }
    //if someone was found
    else {
    fillTable(ui->contr_from_staff_tb, query);
    ui->error_lb->setText("");
    }
}

//refresh data in table
void FireContractorFromStaff::on_refresh_bt_clicked(){
    QString query;
    query = "SELECT st.id_staff, f.surname, f.name, f.id_contractor, p.name_of_position, p.id_position FROM physical_person AS f, position AS p, staff AS st "
            "WHERE st.id_contractor = f.id_contractor AND p.id_position = st.id_position "
            "AND st.id_staff IN (SELECT id_staff FROM cadre_on_position WHERE date_of_leaving_from_position IS NULL);";
    fillTable(ui->contr_from_staff_tb, query);
}

//making table transparent
void FireContractorFromStaff::paintEvent(QPaintEvent *) {
    ui->contr_from_staff_tb->setStyleSheet("background-color: transparent; border : 0 ");
}
