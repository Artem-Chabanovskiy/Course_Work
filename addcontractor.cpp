#include "addcontractor.h"
#include "ui_addcontractor.h"
#include <QDialog>
#include <QLineEdit>
#include <string>

addcontractor::addcontractor(QWidget *parent, QSqlDatabase db1) : QDialog(parent), ui(new Ui::addcontractor) {
    db = db1;
    ui->setupUi(this);
    connect( ui->cansel_button, SIGNAL( clicked() ), SLOT( reject()  ) );
    connect(ui->add_button, SIGNAL ( clicked() ), SLOT( ContractorInsert()));


    ui->surname_edit->setValidator(new QRegExpValidator(QRegExp("[A-Za-zА-Яа-яі]+"), this));
    ui->name_edit->setValidator(new QRegExpValidator(QRegExp("[A-Za-zА-Яа-яі]+"), this));
    ui->pathronymic_edit->setValidator(new QRegExpValidator(QRegExp("[A-Za-zА-Яа-яі]+"), this));
    ui->phone_number_edit->setValidator(new QRegExpValidator(QRegExp("[0-9]+"), this));
    ui->ident_code_edit->setValidator(new QRegExpValidator(QRegExp("[0-9]+"), this));
    //ui->ident_code_edit->setInputMask("DDDDDDDDDD");
}

addcontractor::~addcontractor(){
    delete ui;
}

int addcontractor::ContractorInsert(){
    QString query_function;
    QString name, surname, adress, pathronymic;
    QString id_code, phone_number, date_rozh;
    name = ui->name_edit->text();
    surname = ui->surname_edit->text();
    adress = ui->adress_edit->text();
    pathronymic = ui->pathronymic_edit->text();
    id_code = ui->ident_code_edit->text();
    phone_number =ui->phone_number_edit->text();
    date_rozh = ui->date_of_birth_edit->text();



    query_function = QString("SELECT ""add_contractor_to_physical_person""('%1', '%2', '%3', '%4', '%5', '%6', '%7');")
            .arg(name, surname, pathronymic, adress, id_code, phone_number, date_rozh);
    QSqlQuery*  query = new QSqlQuery;
    query->exec(query_function);
    query->next();
    qDebug() << query_function;
    qDebug() << db.lastError();

    if (query->lastError().isValid()) {
        ui->error_label->setStyleSheet("QLabel { color : red; }");
        std::string s = query->lastError().text().toUtf8().constData();;
        s = s.substr(0,s.find("(P0001)"));
        ui->error_label->setWordWrap(true);
        ui->error_label->setText(QString::fromUtf8(s.c_str()));
    }
    else {
        addcontractor::close();
    }
    return 0;
  }





