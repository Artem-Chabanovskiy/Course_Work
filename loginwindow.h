#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QWidget>
#include <QPicture>
#include <QImage>
#include <QPainter>
#include <QDialog>

namespace Ui {
class LoginWindow;
}

class LoginWindow : public QDialog {
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = 0);
    explicit LoginWindow(QWidget *parent = 0, QString El = "");
    ~LoginWindow();

    QString getLogin();
    QString getPw();
    QString getIP();
    QString getPort();

    Ui::LoginWindow *ui;

protected:
     void paintEvent(QPaintEvent *);

//private:
    //Ui::LoginWindow *ui;

};

#endif // LOGINWINDOW_H
