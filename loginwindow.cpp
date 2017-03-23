#include "loginwindow.h"
#include "ui_loginwindow.h"
#include <QDialogButtonBox>
#include <QDialog>
#include <QDesktopWidget>


/*LoginWindow::LoginWindow(QWidget *parent) :QWidget(parent), ui(new Ui::LoginWindow){
    ui->setupUi(this);
}*/

LoginWindow::LoginWindow(QWidget *parent, QString El) :QDialog(parent), ui(new Ui::LoginWindow) {
    setModal(true);
    ui->setupUi(this);
    ui->error_label->setText(El);

    connect( ui->enter_button, SIGNAL( clicked() ), SLOT( accept() ) );
    connect( ui->exit_button, SIGNAL( clicked() ), SLOT( reject()  ) );

    ui->login_edit->setText("postgres");
    ui->password_edit->setText("12345");
    ui->ip_edit->setText("localhost");
    ui->port_edit->setText("5432");

    QDesktopWidget desktop;
    QRect rect = desktop.availableGeometry(desktop.primaryScreen()); // прямоугольник с размерами экрана
    QPoint center = rect.center(); //координаты центра экрана
    int x = center.x() - (this->width()/2);  // учитываем половину ширины окна
    int y = center.y() - (this->height()/2); // .. половину высоты
    center.setX(x);
    center.setY(y);
    this->move(center);

}

LoginWindow::~LoginWindow() {
    delete ui;
}

QString LoginWindow::getLogin() {
    return ui->login_edit->text();
}

QString LoginWindow::getPw() {
    return ui->password_edit->text();
}

QString LoginWindow::getIP(){
    return ui->ip_edit->text();
}

QString LoginWindow::getPort() {
    return ui->port_edit->text();
}





void LoginWindow::paintEvent(QPaintEvent *) {
    QImage img("C:/megan_2.png");
    QPainter painter(this);
    painter.drawImage(0,0, img.scaled(this->size()));
    //ui->name_label->setStyleSheet("color: rgb(255,000,000)");
    //ui->logwin_label->setStyleSheet("color: rgb(255,000,000)")
    ui->log_label->setStyleSheet("color: rgb(255,250,250)");
    ui->pass_label->setStyleSheet("color: rgb(255,250,250)");
    ui->ip_label->setStyleSheet("color: rgb(255,250,250)");
    ui->port_label->setStyleSheet("color: rgb(255,250,250)");
   // ui->type_label->setStyleSheet("color: rgb(255,250,250)");
   // ui->hr_button->setStyleSheet("color: rgb(255,250,250)");
   // ui->merch_button->setStyleSheet("color: rgb(255,250,250)");
    ui->error_label->setStyleSheet("color: rgb(255,250,250)");
}
