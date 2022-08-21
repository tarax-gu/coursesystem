#include "input_activity.h"
#include "ui_input_activity.h"

input_activity::input_activity(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::input_activity)
{
    ui->setupUi(this);
}

input_activity::~input_activity()
{
    delete ui;
}

void input_activity::on_btn_cancel_clicked()
{
    this->hide();
}

void input_activity::on_btn_yes_clicked()
{
    vtime start_time;
    vtime end_time;
    location act_loc;
    start_time.day_of_week=ui->line_DoW->text();
    start_time.hour=ui->line_start_hour->text();
    start_time.minute=ui->line_start_minute->text();
    start_time.year =ui->line_year->text();
    start_time.month =ui->line_month->text();
    start_time.day =ui->line_day->text();
    start_time.week =ui->line_week->text();
    end_time.day_of_week=ui->line_DoW->text();
    end_time.hour=ui->line_end_hour->text();
    end_time.minute=ui->line_end_minute->text();
    end_time.year =ui->line_year->text();
    end_time.month =ui->line_month->text();
    end_time.day =ui->line_day->text();
    end_time.week =ui->line_week->text();
    act_loc.n =ui->line_n->text().toInt();
    act_loc.campus=ui->line_campus->text();
    act_loc.position=ui->line_position->text();
    item.get_value(ui->line_name->text(),ui->line_type->text(),act_loc,start_time,end_time);
    this->hide();
    emit is_closed();
    ui->line_n->clear();
    ui->line_DoW->clear();
    ui->line_name->clear();
    ui->line_campus->clear();
    ui->line_end_hour->clear();
    ui->line_position->clear();
    ui->line_end_minute->clear();
    ui->line_start_hour->clear();
    ui->line_start_minute->clear();
    ui->line_year->clear();
    ui->line_month->clear();
    ui->line_day->clear();
    ui->line_week->clear();
    ui->line_type->clear();
}
