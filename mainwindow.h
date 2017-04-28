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
    void on_exit_main_button_clicked();
    void on_exit_to_log_w_clicked();

    int showAddContactorWindow();
    int showInterviewWindow();
    int showMovePositionWindow();
    int showAddPositionWindow();
    int showAddContractorToStaffWindow();
    int showAddAbsenceWindow();
    int showFireContrWindow();
    int showRejectContrIntWindow();
    int showReportHR();

    void on_sort_contractor_surname_bt_clicked();

    void on_sort_contractor_date_bt_clicked();

    void on_sort_interview_surname_bt__clicked();

    void on_sort_interview_position_bt_clicked();

    void on_sort_position_surname_bt_clicked();

    void on_sort_position_name_bt_clicked();

    void on_sort_absence_surname_bt_clicked();

    void on_sort_absence_type_bt_clicked();

    void on_search_contr_bt_clicked();

    void on_search_inter_b_clicked();

    void on_search_staff_bt_clicked();

    void on_search_absenc_bt_clicked();

    void on_refresh_bt_clicked();

    void on_add_pos_info_show_clicked();

    void on_add_position_info_hide_clicked();

    void on_full_absence_info_bt_clicked();

    void on_full_absence_info_hide_clicked();

private:
    Ui::MainWindow *ui;
    QString login;
    QString pw;
    QString ip = "localhost";
    int port = 5432;
    QString role;
    //QStringList slContr;
    //QStringList slInter;
    //QStringList slStaff;
    //QStringList slAbsence;
    //int user_type;

    QString getRole();
    int connectUser();
    int showLoginWindow();
    int initHR();
    int initInspector();
    int initInterviewer();
    int refreshHR();
    int fillTable(QTableWidget *, QString);
    int columnResize();





protected:
     void paintEvent(QPaintEvent *);

};

#endif // MAINWINDOW_H
