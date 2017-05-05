#include "addabsencewindow.h"
#include "ui_addabsencewindow.h"
#include <QItemSelectionModel>
#include <string>
#include <QDate>

AddAbsenceWindow::AddAbsenceWindow (QWidget *parent, QSqlDatabase db1) : QDialog(parent), ui(new Ui::AddAbsenceWindow) {
    db = db1;
    ui->setupUi(this);
    connect( ui->cansel_bt, SIGNAL( clicked() ), SLOT( reject()  ) );
    connect( ui->add_absence_bt, SIGNAL (clicked() ), SLOT( AddAbsence() ) );

    ui->search_contr_to_st_edit->setValidator(new QRegExpValidator(QRegExp("[A-Za-zА-Яа-яі]+"), this));
    QDate date = QDate::currentDate();
    ui->start_date_edit->setDate(date);
    ui->end_date_edit->setDate(date);
    //->end_date_edit->setDate(Qt::DefaultLocaleShortDate);

    for (int i = 0; i < ui->contr_absence_tb->horizontalHeader()->count(); ++i)
    ui->contr_absence_tb->horizontalHeader()->setSectionResizeMode(i, QHeaderView::Stretch);

    QString query;
    query = "SELECT ph.surname, ph.name, p.name_of_position, ph.id_contractor "
            "FROM physical_person AS ph, staff AS st, position AS p "
            "WHERE ph.id_contractor = st.id_contractor AND p.id_position = st.id_position "
            "AND st.id_staff IN (SELECT id_staff FROM cadre_on_position WHERE date_of_leaving_from_position IS NULL AND effective_date_of_the_position <= current_date);";
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
    QItemSelectionModel *selectionModel = ui->contr_absence_tb->selectionModel();
    QModelIndexList selectedRows = selectionModel->selectedRows();
    if (selectedRows.size() != 1) {
        ui->error_label->setText("<html><head/><body><p style=\"color:red;\">"
                              "Помилка !"
                              "Оберіть одного працівника!</p></body></html>");
    }
    else {
        int i = ui->contr_absence_tb->currentRow();
        QString id_contr = ui->contr_absence_tb->item(i,3)->text();
        QString position_name = ui->contr_absence_tb->item(i,2)->text();
        QString absence_name = ui->absence_choose_cb->currentText();
        QString date_start = ui->start_date_edit->text();
        QString date_end = ui->end_date_edit->text();

        query_function = QString("SELECT ""add_staff_absence""('%1', '%2', '%3', '%4', '%5');")
                .arg(id_contr, position_name, date_start, date_end, absence_name);
        //qDebug() << query;
        qDebug() << query_function;
        QSqlQuery*  query = new QSqlQuery;
        query->exec(query_function);
        //query->next();
        //int func_result = query->value(0).toInt();//.toString();
        if (query->lastError().isValid()) {
        ui->error_label->setStyleSheet("QLabel { color : red; }");
        std::string s = query->lastError().text().toUtf8().constData();;
        s = s.substr(0,s.find("(P0001)"));
        ui->error_label->setWordWrap(true);
        ui->error_label->setText(QString::fromUtf8(s.c_str()));
        }
        else {
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
    QStringList slContr;
    QString surname_search;
    QString query;
    surname_search = ui->search_contr_to_st_edit->text();
    query = QString("SELECT ph.surname, ph.name, p.name_of_position, ph.id_contractor "
                    "FROM physical_person AS ph, staff AS st, position AS p "
                    "WHERE ph.id_contractor = st.id_contractor AND p.id_position = st.id_position "
                    "AND ph.surname  ILIKE '%%1%' AND st.id_staff IN (SELECT id_staff FROM cadre_on_position WHERE date_of_leaving_from_position IS NULL);").arg(surname_search);
    QSqlQuery sq = db.exec(query);
    while (sq.next())
        slContr << sq.value(0).toString();
    if (slContr.empty()){
        ui->error_label->setText("<html><head/><body><p style=\"color:red;\">"
                              "Помилка !"
                              "Пошук не дав результатів!</p></body></html>");
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
            "WHERE ph.id_contractor = st.id_contractor AND p.id_position = st.id_position "
            "AND st.id_staff IN (SELECT id_staff FROM cadre_on_position WHERE date_of_leaving_from_position IS NULL);";
    fillTable(ui->contr_absence_tb, query);
}


void AddAbsenceWindow::paintEvent(QPaintEvent *) {

    ui->contr_absence_tb->setStyleSheet("background-color: transparent; border : 0 ");

}

