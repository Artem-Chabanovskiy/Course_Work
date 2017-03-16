#include "addcontractor.h"
#include "ui_addcontractor.h"
#include <QDialog>

addcontractor::addcontractor(QWidget *parent, QSqlDatabase db1) : QDialog(parent), ui(new Ui::addcontractor) {
    db = db1;
    ui->setupUi(this);
    connect( ui->cansel_button, SIGNAL( clicked() ), SLOT( reject()  ) );
    connect(ui->add_button, SIGNAL ( clicked() ), SLOT( ContractorInsert()));
}

addcontractor::~addcontractor(){
    delete ui;
}

int addcontractor::ContractorInsert(){
    QString query;
    QString name, surname, adress, pathronymic;
    QString id_code, phone_number, date_rozh;
    name = ui->name_edit->text();
    surname = ui->surname_edit->text();
    adress = ui->adress_edit->text();
    pathronymic = ui->pathronymic_edit->text();
    id_code = ui->ident_code_edit->text();
    phone_number =ui->phone_number_edit->text();
    date_rozh = ui->date_of_birth_edit->text();
    query = QString("INSERT INTO physical_person (name, surname, pathronymic, date_of_birth, adress, identificational_code, phone_number) "
            "VALUES ('%1', '%2', '%3', '%4', '%5', '%6')").arg(name, surname, pathronymic, date_rozh, adress, id_code, phone_number);
    db.exec(query);
    qDebug() << db.lastError();
    addcontractor::close();
    return 0;
}


