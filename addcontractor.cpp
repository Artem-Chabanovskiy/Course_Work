#include "addcontractor.h"
#include "ui_addcontractor.h"
#include <QDialog>
#include <QLineEdit>

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
    int func_result = query->value(0).toInt();//.toString();
    qDebug() << func_result;
    switch (func_result) {
        case 1:
        ui->error_label->setText("<html><head/><body><p style=\"color:red;\">"
                              "Помилка ! <br>"
                              "Одне з полів порожнє! <br> Перевірте ввведені данні!</p></body></html>");
        break;
        case 2:
        ui->error_label->setText("<html><head/><body><p style=\"color:red;\">"
                              "Помилка ! <br>"
                              "Некорректний номер телефону <br> або ідентифікаційний код!</p></body></html>");
        break;
        case 3:
        ui->error_label->setText("<html><head/><body><p style=\"color:red;\">"
                              "Помилка ! <br>"
                              "Особі немає 18 років!</p></body></html>");
        break;
        case 4:
        ui->error_label->setText("<html><head/><body><p style=\"color:red;\">"
                              "Помилка ! <br>"
                              "Особі більше 70 років!</p></body></html>");
        break;
        case 5:
        ui->error_label->setText("<html><head/><body><p style=\"color:red;\">"
                              "Помилка ! <br>"
                              "Некорректна дата народження!</p></body></html>");
        break;
        case 0:
        addcontractor::close();
    }
    //db.exec(query);



   /*QSqlQuery*  query = new QSqlQuery;

    query.prepare("SELECT ""add_contractor_to_physical_person""(:name, :surname, :pathronymic, :adress, :ident_code, :phone_nubmer, :date_of_birth)");
    query.bindValue(":name", name);
    query.bindValue(":surname", surname);
    query.bindValue(":pathronymic", adress);
    query.bindValue(":adress",          data[2].toInt());
    query.bindValue(":ident_code",          data[2].toInt());
    query.bindValue(":phone_number",          data[2].toInt());
    query.bindValue(":date_of_birth",          data[2].toInt());
    query->exec(query);
    query->next();
    int func_result = query->value(0).toInt();*/


    /*if (name == "" || surname == "" || pathronymic == "" || id_code == "" || phone_number == "")
        ui->error_lb->setText("<html><head/><body><p style=\"color:red;\">"
                              "Помилка ! <br>"
                              "Одне з полів порожнє! <br> Перевірте ввведені данні!</p></body></html>");
    else if (phone_number.length() != 10 || id_code.length() != 10)
        ui->error_lb->setText("<html><head/><body><p style=\"color:red;\">"
                              "Помилка ! <br>"
                              "Некорректний номер телефону <br> або ідентифікаційний код!</p></body></html>");
    query = QString("INSERT INTO physical_person (name, surname, pathronymic, date_of_birth, adress, identificational_code, phone_number) "
            "VALUES ('%1', '%2', '%3', '%4', '%5', '%6', '%7');").arg(name, surname, pathronymic, date_rozh, adress, id_code, phone_number);
    db.exec(query);
    qDebug() << db.lastError();*/
    return 0;
}


