#include "addabsencewindow.h"
#include "ui_addabsencewindow.h"

AddAbsenceWindow::AddAbsenceWindow (QWidget *parent, QSqlDatabase db1) : QDialog(parent), ui(new Ui::AddAbsenceWindow) {
    db = db1;
    ui->setupUi(this);
    connect( ui->cansel_bt, SIGNAL( clicked() ), SLOT( reject()  ) );
    connect( ui->add_absence_bt, SIGNAL (clicked() ), SLOT( AddAbsence() ) );

    QString query;
    query = "SELECT ph.surname, ph.name, p.name_of_position, ph.id_contractor "
            "FROM physical_person AS ph, staff AS st, position AS p "
            "WHERE ph.id_contractor = st.id_contractor AND p.id_position = st.id_position;";
    qDebug() << query;
    fillTable(ui->contr_absence_tb, query);
    ui->contr_absence_tb->setSelectionBehavior(QAbstractItemView::SelectRows);

    ui->contr_absence_tb->setColumnHidden(3, true);

     QSqlQueryModel *model = new QSqlQueryModel(this);
     model->setQuery("SELECT name_of_the_type FROM type_of_absence;");
     ui->absence_choose_cb->setModel(model);
}

AddAbsenceWindow::~AddAbsenceWindow()
{
    delete ui;
}

int AddAbsenceWindow::AddAbsence() {
    QString query_function;
    int i = ui->contr_absence_tb->currentRow();
    QString id_contr = ui->contr_absence_tb->item(i,3)->text();
    QString position_name = ui->contr_absence_tb->item(i,2)->text();
    QString absence_name = ui->absence_choose_cb->currentText();
    QString date_start = ui->start_date_edit->text();
    QString date_end = ui->end_date_edit->text();

    query_function = QString("SELECT ""add_staff_absence""('%1', '%2', '%3', '%4', '%5');")
            .arg(id_contr, position_name, date_start, date_end, absence_name);
    QSqlQuery*  query = new QSqlQuery;
    query->exec(query_function);
    query->next();
    int func_result = query->value(0).toInt();//.toString();

    if (absence_name == "Відпустка") {
        switch (func_result) {
        case 1 :
            ui->error_lb->setText("<html><head/><body><p style=\"color:red;\">"
                                  "Помилка ! <br>"
                                  "Невірна дата початку відпустки!</p></body></html>");
            break;
        case 2:
            ui->error_lb->setText("<html><head/><body><p style=\"color:red;\">"
                                  "Помилка ! <br>"
                                  "Невірна дата кінця відпустки!</p></body></html>");
            break;
        case 3:
            ui->error_lb->setText("<html><head/><body><p style=\"color:red;\">"
                                  "Помилка ! <br>"
                                  "У данного контрагента <br> запланована відпуска в цьому році!</p></body></html>");
            break;
        case 4:
            ui->error_lb->setText("<html><head/><body><p style=\"color:red;\">"
                                  "Помилка ! <br>"
                                  "Некоретне введеня данних <br> дата завершення меньша <br> за дату початку!</p></body></html>");
            break;
        case 5:
            ui->error_lb->setText("<html><head/><body><p style=\"color:red;\">"
                                  "Помилка ! <br>"
                                  "Завелика тривалість відпустки!</p></body></html>");
            break;
        case 6:
            ui->error_lb->setText("<html><head/><body><p style=\"color:red;\">"
                                  "Помилка ! <br>"
                                  "Замала тривалість відпустки!</p></body></html>");
            break;
        case 7:
            ui->error_lb->setText("<html><head/><body><p style=\"color:red;\">"
                                  "Помилка ! <br>"
                                  "Відпустка накладається з попередньою!</p></body></html>");
            break;
        case 0:
            AddAbsenceWindow::close();
        }
    }

    if (absence_name == "Лікарняний") {
        switch (func_result) {
        case 1 :
            ui->error_lb->setText("<html><head/><body><p style=\"color:red;\">"
                                  "Помилка ! <br>"
                                  "Невірна дата початку лікарняного!</p></body></html>");
            break;
        case 2:
            ui->error_lb->setText("<html><head/><body><p style=\"color:red;\">"
                                  "Помилка ! <br>"
                                  "Невірна дата кінця лікарняного!</p></body></html>");
            break;
        case 3:
            ui->error_lb->setText("<html><head/><body><p style=\"color:red;\">"
                                  "Помилка ! <br>"
                                  "Некоретне введеня данних <br> Дата завершення меньша <br> за дату початку!</p></body></html>");
            break;
        case 4:
            ui->error_lb->setText("<html><head/><body><p style=\"color:red;\">"
                                  "Помилка ! <br>"
                                  "Завелика тривалість лікарняного <br> (надо увольнять)!</p></body></html>");
            break;
        case 5:
            ui->error_lb->setText("<html><head/><body><p style=\"color:red;\">"
                                  "Помилка ! <br>"
                                  "Лікарняний накладаеться  <br> з минулим <br> лікарняним або відпусткою!</p></body></html>");
            break;
        case 0:
            AddAbsenceWindow::close();
        }
    }
    return 0;
}

int AddAbsenceWindow::fillTable(QTableWidget *tab, QString query){

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

void AddAbsenceWindow::on_absence_sort_bt_clicked()
{
    ui->contr_absence_tb->sortItems(0);
}

void AddAbsenceWindow::on_search_contr_to_st_bt_clicked()
{
    QString surname_search;
    QString query;
    surname_search = ui->search_contr_to_st_edit->text();
    query = QString("SELECT ph.surname, ph.name, p.name_of_position, ph.id_contractor "
                    "FROM physical_person AS ph, staff AS st, position AS p "
                    "WHERE ph.id_contractor = st.id_contractor AND p.id_position = st.id_position AND ph.surname = '%1';").arg(surname_search);
    QSqlQuery sq = db.exec(query);
    while (sq.next())
        slContr << sq.value(0).toString();
    if (!slContr.contains(surname_search)){
        ui->error_label->setText("<html><head/><body><p style=\"color:red;\">"
                              "Помилка ! <br>"
                              "Такої людини намає  <br> в базі!</p></body></html>");
    }
    else {
    fillTable(ui->contr_absence_tb, query);
    ui->error_label->setText("");
    }
}

void AddAbsenceWindow::on_refresh_bt_clicked()
{
    QString query;
    query = "SELECT ph.surname, ph.name, p.name_of_position, ph.id_contractor "
            "FROM physical_person AS ph, staff AS st, position AS p "
            "WHERE ph.id_contractor = st.id_contractor AND p.id_position = st.id_position;";
    fillTable(ui->contr_absence_tb, query);
}
