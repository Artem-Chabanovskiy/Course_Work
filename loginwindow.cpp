#include "loginwindow.h"
#include "ui_loginwindow.h"
#include <QDialogButtonBox>
#include <QDialog>
#include <QDesktopWidget>
#include <QLineEdit>
#include <QtCore>
#include <QDebug>


LoginWindow::LoginWindow(QWidget *parent) :QDialog(parent), ui(new Ui::LoginWindow) {

    setModal(true);
    ui->setupUi(this);

    //connecting buttons to slot
    connect( ui->enter_button, SIGNAL( clicked() ), SLOT( accept() ) );
    connect( ui->exit_button, SIGNAL( clicked() ), SLOT( reject()  ) );

    //setting existing username and name for testing
    ui->login_edit->setText("staff_manager_1");
    ui->password_edit->setText("stfmng_1");

    ui->password_edit->setEchoMode(QLineEdit::Password);

    //setting loginwindow to the centre of the screen
    QDesktopWidget desktop;
    QRect rect = desktop.availableGeometry(desktop.primaryScreen());
    QPoint center = rect.center();
    int x = center.x() - (this->width()/2);
    int y = center.y() - (this->height()/2);
    center.setX(x);
    center.setY(y);
    this->move(center);
}

LoginWindow::~LoginWindow() {
    delete ui;
}

//getting data about user
QString LoginWindow::getLogin() {
    return ui->login_edit->text();
}

QString LoginWindow::getPw() {
    return ui->password_edit->text();
}

//setting error, if wrong login or password
void LoginWindow::set_error() {

    ui->error_label->setText("<html><head/><body><p style=\"color:red;\">"
                             "Error! <br>"
                             "No such user or wrong password!</p></body></html>");
    //refreshing edits
    ui->password_edit->setText("");
    ui->login_edit->setText("");
}

////just for fun
//void LoginWindow::paintEvent(QPaintEvent *) {
//    QImage img("C:/megan_2.png");
//    QPainter painter(this);
//    painter.drawImage(0,0, img.scaled(this->size()));
//    ui->log_label->setStyleSheet("color: rgb(255,250,250)");
//    ui->pass_label->setStyleSheet("color: rgb(255,250,250)");
//    ui->error_label->setStyleSheet("color: rgb(255,250,250)");
//}
