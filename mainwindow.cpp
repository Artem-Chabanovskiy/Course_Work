#include "mainwindow.h"
#include "loginwindow.h"
#include "ui_loginwindow.h"
#include "ui_mainwindow.h"
#include "interviewwindow.h"
#include "moveposotionwindow.h"
#include "addposition.h"
#include "addcontractortostaff.h"
#include "addabsencewindow.h"
#include "firecontractorfromstaff.h"
#include "rejectconrinterview.h"
#include "ncreport.h"
#include "ncreportoutput.h"
#include "ncreportpreviewoutput.h"
#include "ncreportpreviewwindow.h"

#include <QResource>
#include <QMessageBox>
#include <QtCore>
#include <QTabWidget>
#include <QDialog>
#include <QSizePolicy>
#include <QWidget>


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    db = QSqlDatabase::addDatabase("QPSQL");

    //login to database
    showLoginWindow();

    //getting user's role and initialize interface, depending on role
    role = getRole();
    if (role == "staff_manager") initHR();
    if (role == "inspector") initInspector();
    if (role == "interviewer") initInterviewer();

}

MainWindow::~MainWindow() {
    delete ui;
}

int MainWindow::showLoginWindow(){

    LoginWindow d(this);

    //untill user connects ot exits do
    while (1){
        d.show();
        //if "connect" button clicked
        if(d.exec() == QDialog::Accepted){
            login = d.getLogin();
            pw = d.getPw();
            //if connected
            if (connectUser()){
                return 0;
            }
            else
                d.set_error();
        } //if "exit" button clicked
        else{
            qApp->~QApplication();
            break;
        }
    }
    return 1;
}

//getting user's role
QString MainWindow::getRole() {
    QSqlQuery sq = db.exec(QString("select rolname from pg_user\
                            join pg_auth_members on (pg_user.usesysid=pg_auth_members.member)\
                            join pg_roles on (pg_roles.oid=pg_auth_members.roleid)\
                            where pg_user.usename='%1';").arg(login));

    sq.first();
    return sq.value(0).toString();
}

//connecting to DB
int MainWindow::connectUser() {
    db.setHostName(ip);
    db.setPort(port);
    db.setDatabaseName("automotive_parts_shop");
    db.setUserName(login);
    db.setPassword(pw);

    if(db.open()){
        return 1;
    }
    qDebug() << db.lastError();
    return 0;
}


int MainWindow::initHR(){

    ui->hr_w->setVisible(true);

    //additional info
    ui->hr_w->setStyleSheet("QTabWidget::pane {border: 0px; background-repeat: no repeat;}");
    ui->info_label2->setText("HR");

    //refreshing info
    refresh();

    //resisizing columns
    ui->contactor_table->resizeColumnsToContents();
    ui->position_table->resizeColumnsToContents();
    ui->interview_table->resizeColumnsToContents();
    ui->staff_table->resizeColumnsToContents();

    //validators
    ui->search_contr_edit->setValidator(new QRegExpValidator(QRegExp("[A-Za-zА-Яа-яі]+"), this));
    ui->search_absence_edit->setValidator(new QRegExpValidator(QRegExp("[A-Za-zА-Яа-яі]+"), this));
    ui->search_staff_edit->setValidator(new QRegExpValidator(QRegExp("[A-Za-zА-Яа-яі]+"), this));
    ui->search_inter_edit->setValidator(new QRegExpValidator(QRegExp("[A-Za-zА-Яа-яі]+"), this));

    //connecting buttons to slots
    connect(ui->interwiev_do_bt, SIGNAL(clicked()), this, SLOT(showInterviewWindow()));
    connect(ui->position_move_bt, SIGNAL(clicked()), this, SLOT(showMovePositionWindow()));
    connect(ui->positionadd_bt, SIGNAL(clicked()), this, SLOT(showAddPositionWindow()));
    connect(ui->add_contr_omn_staff_bt, SIGNAL(clicked()), this, SLOT(showAddContractorToStaffWindow()));
    connect(ui->absence_add_bt, SIGNAL (clicked()), this, SLOT(showAddAbsenceWindow()));
    connect(ui->fire_contr_from_staff_bt, SIGNAL (clicked()), this, SLOT (showFireContrWindow()));
    connect(ui->report_bt,  SIGNAL (clicked()), this, SLOT (showReportHR()));
    connect(ui->reject_contr_int_bt, SIGNAL (clicked()), this, SLOT (showRejectContrIntWindow()));

    //hiding additional tables
    ui->add_pos_info_tableble->setVisible(false);
    ui->add_pos_info_tableble->setVisible(false);

    //confuguring tables
    ui->contactor_table->setColumnHidden(7, true);
    ui->interview_table->setColumnHidden(3, true);
    ui->staff_table->setColumnHidden(2, true);
    ui->absence_tb->setColumnHidden(2, true);

    //inserting types of absence into combobox
    QSqlQueryModel *model = new QSqlQueryModel(this);
    model->setQuery("SELECT name_of_the_type FROM type_of_absence;");
    ui->full_absence_info_cb->setModel(model);

    return 0;
}




int MainWindow::initInspector() {

    ui->hr_w->setVisible(true);

    //additional info
    ui->hr_w->setStyleSheet("QTabWidget::pane {border: 0px; background-repeat: no repeat;}");
    ui->info_label2->setText("Inspector");

    //refreshing info
    refresh();

    //resisizing columns
    ui->contactor_table->resizeColumnsToContents();
    ui->position_table->resizeColumnsToContents();
    ui->interview_table->resizeColumnsToContents();
    ui->staff_table->resizeColumnsToContents();

    //connecting buttons to slots
    connect(ui->report_bt,  SIGNAL (clicked()), this, SLOT (showReportHR()));

    //validators
    ui->search_contr_edit->setValidator(new QRegExpValidator(QRegExp("[A-Za-zА-Яа-яі]+"), this));
    ui->search_absence_edit->setValidator(new QRegExpValidator(QRegExp("[A-Za-zА-Яа-яі]+"), this));
    ui->search_staff_edit->setValidator(new QRegExpValidator(QRegExp("[A-Za-zА-Яа-яі]+"), this));
    ui->search_inter_edit->setValidator(new QRegExpValidator(QRegExp("[A-Za-zА-Яа-яі]+"), this));

    //confuguring tables
    ui->contactor_table->setColumnHidden(7, true);
    ui->interview_table->setColumnHidden(3, true);
    ui->staff_table->setColumnHidden(2, true);
    ui->absence_tb->setColumnHidden(2, true);

    //hiding unneeded "staff"
    ui->add_pos_info_tableble->setVisible(false);
    ui->fire_contr_from_staff_bt->setVisible(false);
    ui->add_contr_omn_staff_bt->setVisible(false);
    ui->interwiev_do_bt->setVisible(false);
    ui->reject_contr_int_bt->setVisible(false);
    ui->positionadd_bt->setVisible(false);
    ui->position_move_bt->setVisible(false);
    ui->absence_add_bt->setVisible(false);

     //inserting types of absence into combobox
    QSqlQueryModel *model = new QSqlQueryModel(this);
    model->setQuery("SELECT name_of_the_type FROM type_of_absence;");
    ui->full_absence_info_cb->setModel(model);
    return 0;
}

int MainWindow::initInterviewer(){

    ui->hr_w->setVisible(true);

    //additional info
    ui->hr_w->setStyleSheet("QTabWidget::pane {border: 0px; background-repeat: no repeat;}");
    ui->info_label2->setText("Interviewer");

    //refreshing info
    refresh();

    //resisizing columns
    ui->contactor_table->resizeColumnsToContents();
    ui->position_table->resizeColumnsToContents();
    ui->interview_table->resizeColumnsToContents();
    ui->staff_table->resizeColumnsToContents();

    //validators
    ui->search_contr_edit->setValidator(new QRegExpValidator(QRegExp("[A-Za-zА-Яа-яі]+"), this));
    ui->search_absence_edit->setValidator(new QRegExpValidator(QRegExp("[A-Za-zА-Яа-яі]+"), this));
    ui->search_staff_edit->setValidator(new QRegExpValidator(QRegExp("[A-Za-zА-Яа-яі]+"), this));
    ui->search_inter_edit->setValidator(new QRegExpValidator(QRegExp("[A-Za-zА-Яа-яі]+"), this));

    //connecting buttons to slots
    connect(ui->interwiev_do_bt, SIGNAL(clicked()), this, SLOT(showInterviewWindow()));
    connect(ui->add_contr_omn_staff_bt, SIGNAL(clicked()), this, SLOT(showAddContractorToStaffWindow()));
    connect(ui->fire_contr_from_staff_bt, SIGNAL (clicked()), this, SLOT (showFireContrWindow()));
    connect(ui->reject_contr_int_bt, SIGNAL (clicked()), this, SLOT (showRejectContrIntWindow()));

    //confuguring tables
    ui->contactor_table->setColumnHidden(7, true);
    ui->interview_table->setColumnHidden(3, true);
    ui->staff_table->setColumnHidden(2, true);
    ui->absence_tb->setColumnHidden(2, true);
    ui->hr_w->setTabEnabled(4, false);

    //hiding unneeded "staff"
    ui->add_pos_info_tableble->setVisible(false);
    ui->fire_contr_from_staff_bt->setVisible(false);
    ui->add_contr_omn_staff_bt->setVisible(true);
    ui->interwiev_do_bt->setVisible(true);
    ui->positionadd_bt->setVisible(false);
    ui->position_move_bt->setVisible(false);

    //inserting types of absence into combobox
    QSqlQueryModel *model = new QSqlQueryModel(this);
    model->setQuery("SELECT name_of_the_type FROM type_of_absence;");
    ui->full_absence_info_cb->setModel(model);

    return 0;
}



//refreshing info
int MainWindow::refresh(){

    QString query;

    query = "SELECT * FROM physical_person;";
    fillTable(ui->contactor_table, query);

    query = "SELECT (name_of_position) FROM position;";
    fillTable(ui->position_table , query);

    query = "SELECT a.surname AS surname, a.name AS namee, a.pathronymic AS pathronymic, a.id_contractor AS idc, p.name_of_position AS name_position, i.result AS resultat "
            "FROM physical_person a, position p, interview i "
            "WHERE a.id_contractor = i.id_contractor AND p.id_position = i.id_position;";
    fillTable(ui->interview_table, query);

    query = "SELECT a.surname AS surname, a.name AS name, a.id_contractor AS idc, p.name_of_position AS position, cp.cadre_salary AS salary, cp.effective_date_of_the_position AS start_date "
            "FROM physical_person a, position p, staff st, cadre_on_position cp "
            "WHERE a.id_contractor = st.id_contractor AND p.id_position = st.id_position "
            "AND st.id_staff IN (SELECT id_staff FROM cadre_on_position WHERE date_of_leaving_from_position IS NULL) "
            "AND st.id_staff = cp.id_staff AND cp.date_of_leaving_from_position IS NULL;";
    fillTable(ui->staff_table, query);

    query = "SELECT ph.surname, ph.name, ph.id_contractor, p.name_of_position, sa.start_date, sa.end_date, ta.name_of_the_type "
            "FROM physical_person AS ph, position AS p, staff_absence AS sa, type_of_absence AS ta, staff AS st "
            "WHERE sa.id_type_of_absence = ta.id_type_of_absence AND p.id_position = sa.id_position AND sa.id_staff = st.id_staff "
            "AND st.id_contractor = ph.id_contractor;";
    fillTable(ui->absence_tb, query);

    columnResize();

    return 0;
}

/*
 * Window initializing
*/

int MainWindow::showInterviewWindow(){

    interviewwindow iw(this, db);
    iw.setModal(true);
    MainWindow::setVisible(false);
    iw.show();
    int iwres = iw.exec();
    if (QDialog::Rejected)
        iw.close();
    refresh();
    MainWindow::setVisible(true);
    return 1;
}

int MainWindow::showMovePositionWindow(){
    moveposotionwindow mw(this, db);
    mw.setModal(true);
    MainWindow::setVisible(false);
    mw.show();
    int mwres = mw.exec();
    if (QDialog::Rejected)
        mw.close();
    refresh();
    MainWindow::setVisible(true);
    return 1;
}

int MainWindow::showAddContractorToStaffWindow() {
    AddContractorToStaff acs(this, db);
    acs.setModal(true);
    MainWindow::setVisible(false);
    acs.show();
    int acsres = acs.exec();
    if (QDialog::Rejected)
        acs.close();
    refresh();
    MainWindow::setVisible(true);
    return 1;
}

int MainWindow::showFireContrWindow(){
    FireContractorFromStaff fc(this, db);
    fc.setModal(true);
    MainWindow::setVisible(false);
    fc.show();
    int fcres = fc.exec();
    if (QDialog::Rejected)
        fc.close();
    refresh();
    MainWindow::setVisible(true);
    return 1;
}

int MainWindow::showAddPositionWindow() {
    AddPosition ap(this, db);
    ap.setModal(true);
    MainWindow::setVisible(false);
    ap.show();
    int apres = ap.exec();
    if (QDialog::Rejected)
        ap.close();
    refresh();
    MainWindow::setVisible(true);
    return 1;
}

int MainWindow::showAddAbsenceWindow() {
    AddAbsenceWindow aa(this, db);
    aa.setModal(true);
    MainWindow::setVisible(false);
    aa.show();
    int aares = aa.exec();
    if (QDialog::Rejected)
        aa.close();
    refresh();
    MainWindow::setVisible(true);
    return 1;
}

int MainWindow::showRejectContrIntWindow(){

    RejectConrInterview ri(this, db);
    ri.setModal(true);
    MainWindow::setVisible(false);
    ri.show();
    int rires = ri.exec();
    if (QDialog::Rejected)
        ri.close();
    refresh();
    MainWindow::setVisible(true);
    return 1;
}


/*
 * end window initializing
 */


//fillTable function
int MainWindow::fillTable(QTableWidget *tab, QString query){

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

//making tables transparent
void MainWindow::paintEvent(QPaintEvent *) {
    ui->position_table->setStyleSheet("background-color: transparent; border : 0 ");
    ui->interview_table->setStyleSheet("background-color: transparent; border : 0 ");
    ui->contactor_table->setStyleSheet("background-color: transparent; border : 0 ");
    ui->add_pos_info_tableble->setStyleSheet("background-color: transparent; border : 0 ");
    ui->absence_tb->setStyleSheet("background-color: transparent; border : 0 ");
    ui->staff_table->setStyleSheet("background-color: transparent; border : 0 ");
}

//exit to login window from main
void MainWindow::on_exit_to_log_w_clicked(){
    QProcess::startDetached(QApplication::applicationFilePath(), QStringList(), QApplication::applicationDirPath());
    qApp->~QApplication();
}

//report view
int MainWindow::showReportHR(){

    //creating query and exec. function
    QStringList list;
    QString query;
    QString absence_name = ui->full_absence_info_cb->currentText();
    query = QString("SELECT * FROM staff_absence_full_info('%1');").arg(absence_name);
    QSqlQuery sq = db.exec(query);

    //putting info in list
    QString res;
    while (sq.next()){
        res = "";
        for(int i=0; i<6 ;i++)
            res += sq.value(i).toString() + "; ";
        list << res;
    }

   //creating report
   NCReport *report = new NCReport();
   report->setReportFile("C:/report.xml");
   report->addStringList(list, "model1");
   report->runReportToPreview();

   //if report has error - showing info about error
   if (report->hasError()) {
       qDebug() << "ERROR:" << report->lastErrorMsg();
       ui->report_error_lb->setStyleSheet("QLabel { color : red; }");
       ui->report_error_lb->setText("No іnfo about this absence type");
   } else {
       //showing report file
       ui->report_error_lb->setText("");
       NCReportPreviewWindow *pv = new NCReportPreviewWindow();
       pv->setOutput( (NCReportPreviewOutput*)report->output() );
       pv->setWindowModality( Qt::ApplicationModal );
       pv->setAttribute( Qt::WA_DeleteOnClose );
       pv->show();
    }

    delete report;
    return 0;
}


void MainWindow::on_exit_main_button_clicked() {
    QApplication::exit();
}

/*
 * Sort buttons' methods
 */

void MainWindow::on_sort_contractor_surname_bt_clicked() {
    ui->contactor_table->sortItems(0);
}

void MainWindow::on_sort_contractor_date_bt_clicked() {
    ui->contactor_table->sortItems(3);
}

void MainWindow::on_sort_interview_surname_bt__clicked() {
    ui->interview_table->sortItems(0);
}

void MainWindow::on_sort_interview_position_bt_clicked() {
    ui->interview_table->sortItems(4);
}

void MainWindow::on_sort_position_surname_bt_clicked() {
    ui->staff_table->sortItems(0);
}

void MainWindow::on_sort_position_name_bt_clicked() {
    ui->staff_table->sortItems(3);
}

void MainWindow::on_sort_absence_surname_bt_clicked() {
    ui->absence_tb->sortItems(0);
}

void MainWindow::on_sort_absence_type_bt_clicked()
{
    ui->absence_tb->sortItems(6);
}

/*
 * End sort methods
 */

/*
 * Search buttons' methods
 */

void MainWindow::on_search_contr_bt_clicked()
{
    QStringList slContr;
    QString surname_search;
    QString query;
    surname_search = ui->search_contr_edit->text();
    query = QString("SELECT * FROM physical_person WHERE surname ILIKE '%%1%';").arg(surname_search);
    qDebug() << query;
    QSqlQuery sq = db.exec(query);
    while (sq.next())
        slContr << sq.value(0).toString();
        qDebug() << slContr;
    if (slContr.empty()){
        //ok = false;
        //err += "Працівник з таки ім’ям відсутній в базі даних<br>";
        ui->contr_error_lb->setText("<html><head/><body><p style=\"color:red;\">"
                                    "Error ! <br>"
                                    "No results!</p></body></html>");
    }
    else {
    fillTable(ui->contactor_table, query);
    ui->contr_error_lb->setText("");
    }
}

void MainWindow::on_search_inter_b_clicked()
{
    QStringList slInter;
    QString surname_search;
    QString query;
    surname_search = ui->search_inter_edit->text();
    query = QString("SELECT a.surname AS surname, a.name AS namee, a.pathronymic AS pathronymic, a.id_contractor AS idc, p.name_of_position AS name_position, i.result AS resultat "
            "FROM physical_person a, position p, interview i "
            "WHERE a.id_contractor = i.id_contractor AND p.id_position = i.id_position AND a.surname ILIKE '%%1%';").arg(surname_search);
    QSqlQuery sq = db.exec(query);
    qDebug()<< query;
    while (sq.next())
        slInter << sq.value(0).toString();
    qDebug() << slInter;
    if (slInter.empty()){
        //ok = false;
        //err += "Працівник з таки ім’ям відсутній в базі даних<br>";
        ui->inter_error_lb->setText("<html><head/><body><p style=\"color:red;\">"
                                    "Error ! <br>"
                                    "No results!</p></body></html>");
    }
    else {
    fillTable(ui->interview_table, query);
    ui->inter_error_lb->setText("");
    }
}

void MainWindow::on_search_staff_bt_clicked()
{
    QStringList slStaff;
    QString surname_search;
    QString query;
    surname_search = ui->search_staff_edit->text();
    query = QString("SELECT a.surname AS surname, a.name AS name, a.id_contractor AS idc, p.name_of_position AS position, cp.cadre_salary AS salary "
                    "FROM physical_person a, position p, staff st, cadre_on_position cp "
                    "WHERE a.id_contractor = st.id_contractor AND p.id_position = st.id_position "
                    "AND st.id_staff IN (SELECT id_staff FROM cadre_on_position WHERE date_of_leaving_from_position IS NULL) "
                    "AND st.id_staff = cp.id_staff AND cp.date_of_leaving_from_position IS NULL AND a.surname ILIKE '%%1%';").arg(surname_search);
    QSqlQuery sq = db.exec(query);
    while (sq.next())
        slStaff << sq.value(0).toString();
    if (slStaff.empty()){
        ui->staff_error_lb->setText("<html><head/><body><p style=\"color:red;\">"
                                    "Error ! <br>"
                                    "No results!</p></body></html>");
    }
    else {
    fillTable(ui->staff_table, query);
    ui->staff_error_lb->setText("");
    }
}

void MainWindow::on_search_absenc_bt_clicked()
{
    QStringList slAbsence;
    QString surname_search;
    QString query;
    surname_search = ui->search_absence_edit->text();
    query = QString("SELECT ph.surname, ph.name, ph.id_contractor, p.name_of_position, sa.start_date, sa.end_date, ta.name_of_the_type "
                    "FROM physical_person AS ph, position AS p, staff_absence AS sa, type_of_absence AS ta, staff AS st "
                    "WHERE sa.id_type_of_absence = ta.id_type_of_absence AND p.id_position = sa.id_position AND sa.id_staff = st.id_staff "
                    "AND st.id_contractor = ph.id_contractor AND ph.surname  ILIKE '%%1%';").arg(surname_search);
    QSqlQuery sq = db.exec(query);
    while (sq.next())
        slAbsence << sq.value(0).toString();
    if (slAbsence.empty()){
        ui->absence_error_lb->setText("<html><head/><body><p style=\"color:red;\">"
                              "Error ! <br>"
                              "No results!</p></body></html>");
    }
    else {
    fillTable(ui->absence_tb, query);
    ui->absence_error_lb->setText("");
    }
}


/*
 * End search buttons methods
 */


//refreshing tables and error labels
void MainWindow::on_refresh_bt_clicked(){
    refresh();
    ui->absence_error_lb->setText("");
    ui->contr_error_lb->setText("");
    ui->inter_error_lb->setText("");
    ui->staff_error_lb->setText("");
    ui->report_error_lb->setText("");
}

//showing additional position info
void MainWindow::on_add_pos_info_show_clicked(){
    QString query;
    query = "SELECT name_of_position, staff_complement, occupied, avaliable, min_salary, max_salary FROM staff_position_full_info;";
    ui->add_pos_info_tableble->setVisible(true);
    fillTable(ui->add_pos_info_tableble, query);
}

//hiding additional position info
void MainWindow::on_add_position_info_hide_clicked(){
    ui->add_pos_info_tableble->setVisible(false);
}

//streching columns
int MainWindow::columnResize(){
    for (int i = 0; i < ui->contactor_table->horizontalHeader()->count(); ++i)
    ui->contactor_table->horizontalHeader()->setSectionResizeMode(i, QHeaderView::Stretch);
    for (int i = 0; i < ui->position_table->horizontalHeader()->count(); ++i)
    ui->position_table->horizontalHeader()->setSectionResizeMode(i, QHeaderView::Stretch);
    for (int i = 0; i < ui->interview_table->horizontalHeader()->count(); ++i)
    ui->interview_table->horizontalHeader()->setSectionResizeMode(i, QHeaderView::Stretch);
    for (int i = 0; i < ui->staff_table->horizontalHeader()->count(); ++i)
    ui->staff_table->horizontalHeader()->setSectionResizeMode(i, QHeaderView::Stretch);
    for (int i = 0; i < ui->absence_tb->horizontalHeader()->count(); ++i)
    ui->absence_tb->horizontalHeader()->setSectionResizeMode(i, QHeaderView::Stretch);
    return 0;
}
