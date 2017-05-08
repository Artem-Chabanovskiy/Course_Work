#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSql>
#include <QtDebug>
#include <QTableWidget>
#include <QSizePolicy>

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
    //exit buttons
    void on_exit_main_button_clicked();
    void on_exit_to_log_w_clicked();

    //window buttons
    int showInterviewWindow();
    int showMovePositionWindow();
    int showAddPositionWindow();
    int showAddContractorToStaffWindow();
    int showAddAbsenceWindow();
    int showFireContrWindow();
    int showRejectContrIntWindow();

    //report button
    int showReportHR();

    //sort buttons
    void on_sort_contractor_surname_bt_clicked();
    void on_sort_contractor_date_bt_clicked();
    void on_sort_interview_surname_bt__clicked();
    void on_sort_interview_position_bt_clicked();
    void on_sort_position_surname_bt_clicked();
    void on_sort_position_name_bt_clicked();
    void on_sort_absence_surname_bt_clicked();
    void on_sort_absence_type_bt_clicked();

    //search buttons
    void on_search_contr_bt_clicked();
    void on_search_inter_b_clicked();
    void on_search_staff_bt_clicked();
    void on_search_absenc_bt_clicked();
    void on_refresh_bt_clicked();

    //additional info show/hide buttons
    void on_add_pos_info_show_clicked();
    void on_add_position_info_hide_clicked();

private:
    Ui::MainWindow *ui;

    //DB info
    QString login;
    QString pw;
    QString ip = "localhost";
    int port = 5432;

    //role info
    QString role;
    QString getRole();

    //user and loginWindow slots
    int connectUser();
    int showLoginWindow();

    //init slots
    int initHR();
    int initInspector();
    int initInterviewer();

    //additional functions
    int refresh();
    int fillTable(QTableWidget *, QString);
    int columnResize();

protected:
     void paintEvent(QPaintEvent *);

};

#endif // MAINWINDOW_H
