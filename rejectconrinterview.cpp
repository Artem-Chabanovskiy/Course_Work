#include "rejectconrinterview.h"
#include "ui_rejectconrinterview.h"

RejectConrInterview::RejectConrInterview(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RejectConrInterview)
{
    ui->setupUi(this);
}

RejectConrInterview::~RejectConrInterview()
{
    delete ui;
}
