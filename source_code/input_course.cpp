#include "input_course.h"
#include "ui_input_course.h"
#include <QDebug>

input_course::input_course(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::input_course)
{
    ui->setupUi(this);
}

input_course::~input_course()
{
    delete ui;
}

void input_course::on_btn_cancel_clicked()
{
    this->hide();
}

void input_course::on_btn_yes_clicked()
{
    vtime start_time;
    vtime end_time;
    vtime exam_start_time;
    vtime exam_end_time;
    location class_loc;
    location exam_loc;
    start_time.day_of_week=(QString)ui->line_DoW->text();
    start_time.hour=(QString)ui->line_start_hour->text();
    start_time.minute=(QString)ui->line_start_minute->text();
    end_time.hour=(QString)ui->line_end_hour->text();
    end_time.minute=(QString)ui->line_end_minute->text();
    exam_start_time.year=ui->line_exam_year->text().toInt();
    exam_start_time.month=ui->line_exam_month->text().toInt();
    exam_start_time.day=ui->line_exam_day->text().toInt();
    exam_start_time.hour=(QString)ui->line_exam_start_hour->text();
    exam_start_time.minute=(QString)ui->line_exam_start_minute->text();
    exam_start_time.day_of_week=(QString)ui->line_exam_DoW->text();
    exam_start_time.week=(QString)ui->line_exam_week->text();
    exam_end_time.hour=(QString)ui->line_exam_end_hour->text();
    exam_end_time.minute=(QString)ui->line_exam_end_minute->text();
    class_loc.n=ui->line_n->text().toInt();
    class_loc.position=(QString)ui->line_position->text();
    class_loc.campus=(QString)ui->line_campus->text();
    exam_loc.n=ui->line_exam_n->text().toInt();
    exam_loc.position=(QString)ui->line_exam_position->text();
    exam_loc.campus=(QString)ui->line_exam_campus->text();
    input_c(item,ui->line_name->text(),class_loc,start_time,end_time,ui->line_teacher->text(),ui->line_section->text().toInt(),ui->line_group->text(),exam_start_time,exam_end_time,exam_loc);
    this->hide();
    emit is_closed();
    ui->line_n->clear();
    ui->line_DoW->clear();
    ui->line_name->clear();
    ui->line_group->clear();
    ui->line_campus->clear();
    ui->line_exam_n->clear();
    ui->line_section->clear();
    ui->line_teacher->clear();
    ui->line_end_hour->clear();
    ui->line_exam_DoW->clear();
    ui->line_exam_day->clear();
    ui->line_position->clear();
    ui->line_exam_week->clear();
    ui->line_exam_year->clear();
    ui->line_end_minute->clear();
    ui->line_exam_month->clear();
    ui->line_start_hour->clear();
    ui->line_exam_campus->clear();
    ui->line_start_minute->clear();
    ui->line_exam_end_hour->clear();
    ui->line_exam_position->clear();
    ui->line_exam_end_minute->clear();
    ui->line_exam_start_hour->clear();
    ui->line_exam_start_minute->clear();
}
