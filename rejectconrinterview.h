#ifndef REJECTCONRINTERVIEW_H
#define REJECTCONRINTERVIEW_H

#include <QWidget>

namespace Ui {
class RejectConrInterview;
}

class RejectConrInterview : public QWidget
{
    Q_OBJECT

public:
    explicit RejectConrInterview(QWidget *parent = 0);
    ~RejectConrInterview();

private:
    Ui::RejectConrInterview *ui;
};

#endif // REJECTCONRINTERVIEW_H
