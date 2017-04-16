#include "addposition.h"
#include "ui_addposition.h"
#include <QDialog>

AddPosition::AddPosition(QWidget *parent, QSqlDatabase db1) :
    QDialog(parent),
    ui(new Ui::AddPosition)
{
    ui->setupUi(this);
    db = db1;
    connect( ui->cansel_bt, SIGNAL( clicked() ), SLOT( reject()  ) );
    connect(ui->add_position_bt, SIGNAL ( clicked() ), SLOT( add_position()));

    ui->staff_complement_edit->setValidator(new QRegExpValidator(QRegExp("[0-9]+"), this));
    ui->position_salary_min_edit->setValidator(new QRegExpValidator(QRegExp("[0-9]+"), this));
    ui->position_salary_max_edit->setValidator(new QRegExpValidator(QRegExp("[0-9]+"), this));
    ui->position_name_edit->setValidator(new QRegExpValidator(QRegExp("[A-Za-zА-Яа-яі]+"), this));

}

AddPosition::~AddPosition()
{
    delete ui;
}

int AddPosition::add_position(){
    QString query_function;
    QString name_position, min_payment, max_payment, staff_compl;
    name_position = ui->position_name_edit->text();
    min_payment = ui->position_salary_min_edit->text();
    max_payment = ui->position_salary_max_edit->text();
    staff_compl = ui->staff_complement_edit->text();
    query_function = QString("SELECT ""add_position""('%1', '%2', '%3', '%4');")
            .arg(name_position, min_payment, max_payment, staff_compl);
    qDebug() << query_function;
    QSqlQuery*  query = new QSqlQuery;
    query->exec(query_function);
    qDebug() << query->lastError();
    if (query->lastError().isValid()) {
    ui->error_label->setStyleSheet("QLabel { color : red; }");
    std::string s = query->lastError().text().toUtf8().constData();;
    s = s.substr(0,s.find("(P0001)"));
    ui->error_label->setWordWrap(true);
    ui->error_label->setText(QString::fromUtf8(s.c_str()));
    }
    else {
       AddPosition::close();
    }
    return 0;
}
