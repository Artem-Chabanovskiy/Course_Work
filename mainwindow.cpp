#include "mainwindow.h"
#include "loginwindow.h"
#include "ui_loginwindow.h"
#include "ui_mainwindow.h"
#include <QResource>
#include <QMessageBox>
#include <QtCore>
#include <QTabWidget>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    db = QSqlDatabase::addDatabase("QPSQL");
    showLoginWindow();
    if (user_type == 0) initHR();
    if (user_type == 1) initManager();
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
            user_type = d.getUserType();
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

void MainWindow::on_exit_main_button_clicked() {
    QApplication::quit();
}

void MainWindow::paintEvent(QPaintEvent *) {

    ui->info_label1->setStyleSheet("background-color: gray ");
    ui->info_label2->setStyleSheet("background-color: gray ");
    if (user_type == 0) {
        QImage img("C:/eva.png");
        QPainter painter(this);
        painter.drawImage(0,0, img.scaled(this->size()));
    }
    if (user_type == 1) {
        QImage img("C:/megan_3.png");
        QPainter painter(this);
        painter.drawImage(0,0, img.scaled(this->size()));
    }

}
int MainWindow::connectUser(){
    db.setHostName(ip);
    db.setPort(port.toInt());
    db.setDatabaseName("Automotive_Parts_Shop");
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
    ui->info_label2->setText("HR");
    return 0;
}

int MainWindow::initManager(){
    ui->manag_w->setVisible(true);
    ui->hr_w->setVisible(false);
    ui->manag_w->setStyleSheet("QTabWidget::pane {border: 0px; background-repeat: no repeat;}");
    ui->info_label2->setText("Менеджер");
    return 0;
}
