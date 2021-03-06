#include "addposition.h"
#include "ui_addposition.h"
#include <QDialog>
#include <QString>

AddPosition::AddPosition(QWidget *parent, QSqlDatabase db1) : QDialog(parent), ui(new Ui::AddPosition) {
    ui->setupUi(this);
    db = db1;

    //connecting buttons to slots
    connect( ui->cansel_bt, SIGNAL( clicked() ), SLOT( reject()  ) );
    connect(ui->add_position_bt, SIGNAL ( clicked() ), SLOT( add_position()));

    //validators
    ui->staff_complement_edit->setValidator(new QRegExpValidator(QRegExp("[0-9]+"), this));
    ui->position_salary_min_edit->setValidator(new QRegExpValidator(QRegExp("[0-9]+"), this));
    ui->position_salary_max_edit->setValidator(new QRegExpValidator(QRegExp("[0-9]+"), this));
    ui->position_name_edit->setValidator(new QRegExpValidator(QRegExp("[A-Za-zА-Яа-яі]+"), this));

}

AddPosition::~AddPosition() {
    delete ui;
}

int AddPosition::add_position() {
    QString query_function;
    QString name_position, min_payment, max_payment, staff_compl;
    //selecting needed info from edits
    name_position = ui->position_name_edit->text();
    min_payment = ui->position_salary_min_edit->text();
    max_payment = ui->position_salary_max_edit->text();

    //checking length of min and max payments
    if (min_payment.length() >= 8 || max_payment.length() >= 8) {
        ui->error_label->setText("<html><head/><body><p style=\"color:red;\">"
                                 "ERROR: "
                                 "Wrong salary!</p></body></html>");
    }
    else {
        //creating and exec query
        staff_compl = ui->staff_complement_edit->text();
        query_function = QString("SELECT ""add_position""('%1', '%2', '%3', '%4');")
                .arg(name_position, min_payment, max_payment, staff_compl);
        QSqlQuery*  query = new QSqlQuery;
        query->exec(query_function);
        //if the EXEPTION was RAISED - showing info about error
        if (query->lastError().isValid()) {
            ui->error_label->setStyleSheet("QLabel { color : red; }");
            std::string s = query->lastError().text().toUtf8().constData();;
            s = s.substr(0,s.find("(P0001)"));
            ui->error_label->setWordWrap(true);
            ui->error_label->setText(QString::fromUtf8(s.c_str()));
        } //if transaction commited
        else {
           AddPosition::close();
        }
        return 0;
    }
}
