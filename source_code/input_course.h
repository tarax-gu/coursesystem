#ifndef INPUT_COURSE_H
#define INPUT_COURSE_H

#include <QWidget>
#include "course.h"

namespace Ui {
class input_course;
}

class input_course : public QWidget
{
    Q_OBJECT

public:
    explicit input_course(QWidget *parent = nullptr);
    ~input_course();
    course get_course(){return item;};
signals:
    void is_closed();
private slots:
    void on_btn_cancel_clicked();

    void on_btn_yes_clicked();

private:
    Ui::input_course *ui;
    course item;
};

#endif // INPUT_COURSE_H
