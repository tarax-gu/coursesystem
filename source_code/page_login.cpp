#include "page_login.h"
#include "ui_page_login.h"
#include <QString>

page_login::page_login(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::page_login)
{
    ui->setupUi(this);
}

page_login::~page_login()
{
    delete ui;
}

void page_login::on_btn_login_clicked()
{
    if((ui->lineEdit->text()==QString("student"))&&(ui->lineEdit_2->text()==QString("123456")))
    {
        emit sendStudentLogin();

    }
    else if((ui->lineEdit->text()==QString("admin"))&&(ui->lineEdit_2->text()==QString("123456")))
    {
        emit sendAdminLogin();
    }
    ui->lineEdit->clear();
    ui->lineEdit_2->clear();
}


void page_login::on_pushButton_clicked()
{
    exit(0);
}
