#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSql>
#include <QtDebug>
#include <QTableWidget>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QSqlDatabase db;

private slots:
    void on_exit_main_button_clicked();
    void on_exit_to_log_w_clicked();

    int showAddContactorWindow();
    int showInterviewWindow();
    int showMovePositionWindow();
    int showAddPositionWindow();
    int showDelPositionWindow();
    int showAddContractorToStaffWindow();

private:
    Ui::MainWindow *ui;
    QString login;
    QString pw;
    QString ip;
    QString port;
    int user_type;

    int connectUser();
    int showLoginWindow();
    int initManager();
    int initHR();
    int refreshHR();
    int fillTable(QTableWidget *, QString);





protected:
     void paintEvent(QPaintEvent *);

};

#endif // MAINWINDOW_H
