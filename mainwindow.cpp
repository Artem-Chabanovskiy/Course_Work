#include "mainwindow.h"
#include "loginwindow.h"
#include "ui_loginwindow.h"
#include "ui_mainwindow.h"
#include "addcontractor.h"
#include "interviewwindow.h"
#include "moveposotionwindow.h"
#include "addposition.h"
#include "addcontractortostaff.h"
#include <QResource>
#include <QMessageBox>
#include <QtCore>
#include <QTabWidget>
#include <QDialog>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    db = QSqlDatabase::addDatabase("QPSQL");
    showLoginWindow();
    //if (user_type == 0) initHR();
    //if (user_type == 1) initManager();
    role = getRole();
    if (role == "staff_manager") initHR();
    if (role == "inspector") initInspector();
}

MainWindow::~MainWindow() {
    delete ui;
}

int MainWindow::showLoginWindow(){
    QString El;
    while (1){
        LoginWindow d(this, El);
        d.show();
        if(d.exec() == QDialog::Accepted){
            login = d.getLogin();
            pw = d.getPw();
            ip = d.getIP();
            port = d.getPort();
            //user_type = d.getUserType();
            if (connectUser()){
                return 0;
            }

            else
                El = "<html><head/><body><p style=\"color:red;\">"
                     "Помилка доступу! <br>"
                     "Такого користувача немає в БД!</p></body></html>";
        }
        if(d.exec() == QDialog::Rejected){
            //MainWindow::~MainWindow();
            qApp->~QApplication();
            break;
        }
    }
    return 1;
}

QString MainWindow::getRole(){
    QSqlQuery sq = db.exec(QString("select rolname from pg_user\
                            join pg_auth_members on (pg_user.usesysid=pg_auth_members.member)\
                            join pg_roles on (pg_roles.oid=pg_auth_members.roleid)\
                            where pg_user.usename='%1';").arg(login));

    sq.first();
    return sq.value(0).toString();
}

int MainWindow::connectUser(){
    db.setHostName(ip);
    db.setPort(port.toInt());
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
    ui->manag_w->setVisible(false);
    ui->hr_w->setStyleSheet("QTabWidget::pane {border: 0px; background-repeat: no repeat;}");
    ui->info_label2->setText("Менеджер Персоналу");
    ui->contactor_table->resizeColumnsToContents();
    ui->position_table->resizeColumnsToContents();
    ui->interview_table->resizeColumnsToContents();
    ui->staff_table->resizeColumnsToContents();
    refreshHR();
    connect(ui->add_contr_bt, SIGNAL(clicked()), this, SLOT(showAddContactorWindow()));
    connect(ui->interwiev_do_bt, SIGNAL(clicked()), this, SLOT(showInterviewWindow()));
    connect(ui->position_move_bt, SIGNAL(clicked()), this, SLOT(showMovePositionWindow()));
    connect(ui->positionadd_bt, SIGNAL(clicked()), this, SLOT(showAddPositionWindow()));
    connect(ui->add_contr_omn_staff_bt, SIGNAL(clicked()), this, SLOT(showAddContractorToStaffWindow()));
    return 0;
}

int MainWindow::initInspector(){
    ui->hr_w->setVisible(true);
    ui->manag_w->setVisible(false);
    ui->hr_w->setStyleSheet("QTabWidget::pane {border: 0px; background-repeat: no repeat;}");
    ui->info_label2->setText("Інспектор");
    ui->contactor_table->resizeColumnsToContents();
    ui->position_table->resizeColumnsToContents();
    ui->interview_table->resizeColumnsToContents();
    ui->staff_table->resizeColumnsToContents();
    refreshHR();
    ui->addintresult_bt->setVisible(false);
    ui->add_contr_bt->setVisible(false);
    ui->add_contr_omn_staff_bt->setVisible(false);
    ui->interwiev_do_bt->setVisible(false);
    ui->positionadd_bt->setVisible(false);
    ui->position_move_bt->setVisible(false);
    return 0;
}


int MainWindow::refreshHR(){
    QString query;
    query = "SELECT * FROM physical_person;";
    fillTable(ui->contactor_table, query);
    query = "SELECT (name_of_position) FROM position;";
    fillTable(ui->position_table , query);
    query = "SELECT a.surname AS surname, a.name AS namee, a.pathronymic AS pathronymic,p.name_of_position AS name_position, i.result AS resultat FROM physical_person a, position p, interview i WHERE a.id_contractor = i.id_contractor AND p.id_position = i.id_position;";
    fillTable(ui->interview_table, query);
    query = "SELECT a.surname AS surname, a.name AS name, p.name_of_position AS position FROM physical_person a, position p, staff WHERE a.id_contractor = staff.id_contractor AND p.id_position = staff.id_position;";
    fillTable(ui->staff_table, query);
    return 0;
}

int MainWindow::initManager(){
    ui->manag_w->setVisible(true);
    ui->hr_w->setVisible(false);
    ui->manag_w->setStyleSheet("QTabWidget::pane {border: 0px; background-repeat: no repeat;}");
    ui->info_label2->setText("Менеджер");
    return 0;
}

int MainWindow::showAddContactorWindow() { //gotovo
    addcontractor ac(this, db);
    ac.setModal(true);
    MainWindow::setVisible(false);
    ac.show();
    int acres = ac.exec();
    if (QDialog::Rejected)
        ac.close();
    refreshHR();
    MainWindow::setVisible(true);
    return 1;
}

int MainWindow::showInterviewWindow(){ //next

    interviewwindow iw(this, db);
    iw.setModal(true);
    MainWindow::setVisible(false);
    iw.show();
    int iwres = iw.exec();
    if (QDialog::Rejected)
        iw.close();
    MainWindow::setVisible(true);
    return 1;
}

int MainWindow::showMovePositionWindow(){ //poslednee
    moveposotionwindow mw(this, db);
    mw.setModal(true);
    MainWindow::setVisible(false);
    mw.show();
    int mwres = mw.exec();
    if (QDialog::Rejected)
        mw.close();
    refreshHR();
    MainWindow::setVisible(true);
    return 1;
}

int MainWindow::showAddContractorToStaffWindow() { //posle interview
    AddContractorToStaff acs(this, db);
    acs.setModal(true);
    MainWindow::setVisible(false);
    acs.show();
    int acsres = acs.exec();
    if (QDialog::Rejected)
        acs.close();
    MainWindow::setVisible(true);
    return 1;
}

int MainWindow::showAddPositionWindow() { // gotovo
    AddPosition ap(this, db);
    ap.setModal(true);
    MainWindow::setVisible(false);
    ap.show();
    int apres = ap.exec();
    if (QDialog::Rejected)
        ap.close();
    refreshHR();
    MainWindow::setVisible(true);
    return 1;
}



int MainWindow::fillTable(QTableWidget *tab, QString query){

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

void MainWindow::paintEvent(QPaintEvent *) {

    ui->info_label1->setStyleSheet("background-color: gray ");
    ui->info_label2->setStyleSheet("background-color: gray ");

    if (role == "staff_manager") {
        QImage img("C:/eva.png");
        QPainter painter(this);
        painter.drawImage(0,0, img.scaled(this->size()));
    }

    if (role == "inspector") {
        QImage img("C:/eva_2.png");
        QPainter painter(this);
        painter.drawImage(0,0, img.scaled(this->size()));
    }

    if (role == '228') {
        QImage img("C:/megan_3.png");
        QPainter painter(this);
        painter.drawImage(0,0, img.scaled(this->size()));
    }
}

void MainWindow::on_exit_to_log_w_clicked(){
    QProcess::startDetached(QApplication::applicationFilePath(), QStringList(), QApplication::applicationDirPath());
    qApp->~QApplication();
}


void MainWindow::on_exit_main_button_clicked() {
    QApplication::exit();
}
