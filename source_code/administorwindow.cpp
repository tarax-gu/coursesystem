#include "administorwindow.h"
#include "ui_administorwindow.h"
#include <QFile>
#include <QDebug>
#include <QMessageBox>
#include "input_course.h"
#include <QInputDialog>
#include <QTableWidget>
#include <QTreeWidgetItem>

administerWindow::administerWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::administerWindow)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);
    ui->treeWidget->clear();
    ui->treeWidget->setColumnCount(1);
    QStringList item;
    item <<"课程管理系统";
    QTreeWidgetItem *phead = new QTreeWidgetItem(ui->treeWidget,item);
    ui->treeWidget->addTopLevelItem(phead);
    item.clear();
    item<<"课程信息";
    QTreeWidgetItem *p1 = new QTreeWidgetItem(phead,item);
    item.clear();
    item<<"作业信息";
    QTreeWidgetItem *p2 = new QTreeWidgetItem(phead,item);
    item.clear();
    item<<"日志系统";
    QTreeWidgetItem *p3 = new QTreeWidgetItem(phead,item);
    item.clear();
    phead->addChild(p1);
    phead->addChild(p2);
    phead->addChild(p3);
    ui->treeWidget->expandAll();
    ui->treeWidget->setCurrentItem(p1);
    connect(ui->btn_exit,&QPushButton::clicked,this,[=](){
        this->close();
        emit is_closed();
    });
    connect(&input_c_add,&input_course::is_closed,this,[=](){
        item_c[total_course]=input_c_add.get_course();
        total_course++;
        this->update_table();
        QMessageBox::information(this,tr("提示"),tr("添加完成！"));
        initcoursetable();
        str_log="管理员amdin添加了课程："+input_c_add.get_course().get_course_name();
        emit send_log();
    });
    connect(&input_c_modify,&input_course::is_closed,this,[=](){
        item_c[a[choose]]=input_c_modify.get_course();
        this->update_table();
        QMessageBox::information(this,tr("提示"),tr("修改完成！"));
        emit operate_over();
        str_log="管理员amdin修改了课程："+input_c_modify.get_course().get_course_name();
        emit send_log();
    });
    connect(this,&administerWindow::operate_over,this,&administerWindow::on_btn_return_clicked);
    connect(this,&administerWindow::issue_over,this,&administerWindow::on_btn_return_2_clicked);
    read_course_information();
    initcoursetable();
    inithomeworktable();
}

administerWindow::~administerWindow()
{
    delete ui;
}

void administerWindow::initcoursetable() //课程信息表格初始化
{
    QStringList headers;
    headers << QObject::tr("序号")
            << QObject::tr("课程名称")
            << QObject::tr("任课老师")
            << QObject::tr("上课时间")
            << QObject::tr("上课地点")
            << QObject::tr("课程群")
            << QObject::tr("课程进度")
            << QObject::tr("考试时间")
            << QObject::tr("考试地点");
    this->ui->course_table2->setColumnCount(headers.size()); //设置列数
    this->ui->course_table2->setRowCount(total_course);//设置行数
    this->ui->course_table2->setHorizontalHeaderLabels(headers);//列名称
    this->ui->course_table2->setAlternatingRowColors(true);//行间隔色
    this->ui->course_table2->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);//自动列宽
    this->ui->course_table2->installEventFilter(this);
    for(int row = 0;row < total_course;row++)
    {
        int colindex = 0;
        this->ui->course_table2->setItem(row,colindex++,new QTableWidgetItem(QString::number(row,10)));
        this->ui->course_table2->setItem(row,colindex++,new QTableWidgetItem(item_c[row].get_course_name()));
        this->ui->course_table2->setItem(row,colindex++,new QTableWidgetItem(item_c[row].get_teacher_name()));
        QString str = QString("%1 %2:%3-%4:%5").arg(item_c[row].get_start_time().day_of_week)
                                             .arg(item_c[row].get_start_time().hour)
                                               .arg(item_c[row].get_start_time().minute)
                                               .arg(item_c[row].get_end_time().hour)
                                               .arg(item_c[row].get_end_time().minute);
        this->ui->course_table2->setItem(row,colindex++,new QTableWidgetItem(str));
        str = QString("%1 %2").arg(item_c[row].get_course_location().campus).arg(item_c[row].get_course_location().position);
        this->ui->course_table2->setItem(row,colindex++,new QTableWidgetItem(str));
        this->ui->course_table2->setItem(row,colindex++,new QTableWidgetItem(item_c[row].get_course_group()));
        str = QString("%1/%2").arg(item_c[row].get_cur_section()).arg(item_c[row].get_total_section());
        this->ui->course_table2->setItem(row,colindex++,new QTableWidgetItem(str));
        str = QString("第%1周 %2 %3:%4-%5:%6").arg(item_c[row].get_exam_start_time().week)
                                              .arg(item_c[row].get_exam_start_time().day_of_week)
                                                     .arg(item_c[row].get_exam_start_time().hour)
                                                       .arg(item_c[row].get_exam_start_time().minute)
                                                       .arg(item_c[row].get_exam_end_time().hour)
                                                       .arg(item_c[row].get_exam_end_time().minute);
        this->ui->course_table2->setItem(row,colindex++,new QTableWidgetItem(str));
        str = QString("%1 %2").arg(item_c[row].get_exam_loc().campus).arg(item_c[row].get_exam_loc().position);
        this->ui->course_table2->setItem(row,colindex++,new QTableWidgetItem(str));

    }

}
void administerWindow::inithomeworktable() //课程作业信息表格初始化
{
    QStringList headers;
    headers << QObject::tr("序号")
            << QObject::tr("课程名称")
            << QObject::tr("作业名称")
            << QObject::tr("完成状况");
    this->ui->homework_table->setColumnCount(headers.size()); //设置列数
    this->ui->homework_table->setHorizontalHeaderLabels(headers);//列名称
    this->ui->homework_table->setAlternatingRowColors(true);//行间隔色
    this->ui->homework_table->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);//自动列宽
    this->ui->homework_table->installEventFilter(this);
    int row = 0;
    for(int i = 0;i < total_course;i++)
    {
        int k = 0;
        int flag = 1;
        while(k < i)
        {
            if(item_c[k].get_course_name().compare(item_c[i].get_course_name())==0)
            {
                flag = 0;
            }
            k++;
        }
        if(flag == 1)
        {
            int j =0;
            while(item_c[i].get_homework()[j].now!=-1)
            {
                int colindex = 0;
                if(item_c[i].get_homework()[j].now == 1)
                {
                    this->ui->homework_table->insertRow(row);
                    this->ui->homework_table->setItem(row,colindex++,new QTableWidgetItem(QString::number(row,10)));
                    this->ui->homework_table->setItem(row,colindex++,new QTableWidgetItem(item_c[i].get_course_name()));
                    this->ui->homework_table->setItem(row,colindex++,new QTableWidgetItem(item_c[i].get_homework()[j].workname));
                    if(item_c[i].get_homework()[j].flag == 1){
                        this->ui->homework_table->setItem(row,colindex++,new QTableWidgetItem("已交"));
                    }
                    else if(item_c[i].get_homework()[j].flag == 0){
                        this->ui->homework_table->setItem(row,colindex++,new QTableWidgetItem("未交"));
                    }
                    total_homework++;
                    row++;
                }
                j++;
            }
        }
    }
}
void administerWindow::clear_homework_table()
{
    for(int i=total_homework;i>=0;i--)
    {
        ui->homework_table->removeRow(i);
        total_homework--;
    }
}
void administerWindow::read_course_information()
{
    total_course=0;
    QFile file1("../total_information/course_information.txt");
    bool isOk = file1.open(QIODevice::ReadOnly);
    if(isOk == true)
    {
        QTextStream in(&file1);
        while(!read_course(in,item_c[total_course]).atEnd())
        {
            total_course++;
        }
    }
    else
    {
        qDebug() << "总课程信息文件打开失败";
    }
    file1.close();
}


void administerWindow::on_btn_return_clicked()
{
    ui->course_table2->clear();
    initcoursetable();
    ui->btn_return->setEnabled(false);
    ui->btn_add->setEnabled(true);
    ui->btn_delete->setEnabled(false);
    ui->btn_modify->setEnabled(false);
    ui->line_search->clear();
    num=0;
    str_log="管理员amdin退出了查询课程状态";
    emit send_log();
}

void administerWindow::on_btn_add_clicked()
{
    input_c_add.show();
}

void administerWindow::update_table()
{
    QFile file("../total_information/course_information.txt");
    bool isOk=file.open(QIODevice::WriteOnly);
    if(isOk == true)
    {
        QTextStream out(&file);
        for(int i=0;i<total_course;i++)
        {
            out << item_c[i].get_course_name() << "\n"
                << item_c[i].get_course_location().n << " " << item_c[i].get_course_location().position << " " << item_c[i].get_course_location().campus << "\n"
                << item_c[i].get_start_time().day_of_week << " " << item_c[i].get_start_time().hour << " " << item_c[i].get_start_time().minute << " "
                << item_c[i].get_end_time().hour << " " << item_c[i].get_end_time().minute << "\n"
                << item_c[i].get_teacher_name() << "\n"
                << item_c[i].get_cur_section() << " " << item_c[i].get_total_section() << "\n"
                << item_c[i].get_data_filename() << "\n" << item_c[i].get_work_filename() << "\n"
                << item_c[i].get_course_group() << "\n"
                << item_c[i].get_exam_start_time().year << " " << item_c[i].get_exam_start_time().month << " " << item_c[i].get_exam_start_time().day << "\n"
                << item_c[i].get_exam_start_time().week << " " << item_c[i].get_exam_start_time().day_of_week << " "
                << item_c[i].get_exam_start_time().hour << " " << item_c[i].get_exam_start_time().minute << " "
                << item_c[i].get_exam_end_time().hour << " " << item_c[i].get_exam_end_time().minute << "\n"
                << item_c[i].get_exam_loc().n << " " << item_c[i].get_exam_loc().position << " " << item_c[i].get_exam_loc().campus << "\n";
        }
    }
    else
    {
        qDebug() << "总课程信息文件打开失败";
    }
    file.close();
}

void administerWindow::on_btn_delete_clicked()
{
    bool ok;
    int choose;
    QString text=QInputDialog::getText(this,tr("选择课程"),tr("请输入要删除课程的编号"),QLineEdit::Normal,0,&ok);
    if(ok && !text.isEmpty())
    {
        qDebug() << "开始删除课程";
        choose=text.toInt()-1;
        str_log="管理员amdin删除了课程："+item_c[a[choose]].get_course_name();
        emit send_log();
        for(int j=a[choose];j<total_course-1;j++)
        {
            item_c[j]=item_c[j+1];
        }
        total_course--;
        qDebug() << "删除完成";
        update_table();
        QMessageBox::information(this,tr("提示"),tr("删除完成！"));
        emit operate_over();
    }
}

void administerWindow::on_btn_search_clicked()
{
    if(ui->line_search->text().isEmpty())
    {
        return;
    }
    else
    {
        ui->course_table2->clearContents();
        int row = 0;
        for(int i = 0;i < total_course;i++){
            if(item_c[i].get_course_name() == ui->line_search->text())
            {
                a[num]=i;
                num++;
                int colindex = 0;
                this->ui->course_table2->setItem(row,colindex++,new QTableWidgetItem(QString::number(row+1,10)));
                this->ui->course_table2->setItem(row,colindex++,new QTableWidgetItem(item_c[i].get_course_name()));
                this->ui->course_table2->setItem(row,colindex++,new QTableWidgetItem(item_c[i].get_teacher_name()));
                QString str = QString("%1 %2:%3-%4:%5").arg(item_c[i].get_start_time().day_of_week)
                                                       .arg(item_c[i].get_start_time().hour)
                                                       .arg(item_c[i].get_start_time().minute)
                                                       .arg(item_c[i].get_end_time().hour)
                                                       .arg(item_c[i].get_end_time().minute);
                this->ui->course_table2->setItem(row,colindex++,new QTableWidgetItem(str));
                str = QString("%1 %2").arg(item_c[i].get_course_location().campus).arg(item_c[i].get_course_location().position);
                this->ui->course_table2->setItem(row,colindex++,new QTableWidgetItem(str));
                this->ui->course_table2->setItem(row,colindex++,new QTableWidgetItem(item_c[i].get_course_group()));
                str = QString("%1/%2").arg(item_c[i].get_cur_section()).arg(item_c[i].get_total_section());
                this->ui->course_table2->setItem(row,colindex++,new QTableWidgetItem(str));
                str = QString("第%1周 %2 %3:%4-%5:%6").arg(item_c[i].get_exam_start_time().week)
                                                      .arg(item_c[i].get_exam_start_time().day_of_week)
                                                      .arg(item_c[i].get_exam_start_time().hour)
                                                      .arg(item_c[i].get_exam_start_time().minute)
                                                      .arg(item_c[i].get_exam_end_time().hour)
                                                      .arg(item_c[i].get_exam_end_time().minute);
                this->ui->course_table2->setItem(row,colindex++,new QTableWidgetItem(str));
                str = QString("%1 %2").arg(item_c[row].get_exam_loc().campus).arg(item_c[i].get_exam_loc().position);
                this->ui->course_table2->setItem(row,colindex++,new QTableWidgetItem(str));
                row++;
            }
        }
        if(row == 0){
            QMessageBox::information(this,"提示","该课程不存在");
        }
        else if(row > 0)
        {
            ui->btn_add->setEnabled(false);
            ui->btn_return->setEnabled(true);
            ui->btn_delete->setEnabled(true);
            ui->btn_modify->setEnabled(true);
            num=0;
        }
        str_log="管理员amdin查询了课程："+ui->line_search->text();
        emit send_log();
    }
}

void administerWindow::on_btn_modify_clicked()
{
    bool ok;
    QString text=QInputDialog::getText(this,tr("选择课程"),tr("请输入要修改课程的编号"),QLineEdit::Normal,0,&ok);
    if(ok && !text.isEmpty())
    {
        choose=text.toInt()-1;
        input_c_modify.show();
    }
}

void administerWindow::on_treeWidget_itemClicked(QTreeWidgetItem *item, int column)
{
    if(item->text(column)=="课程信息")
    {
        ui->stackedWidget->setCurrentIndex(0);
        str_log="管理员amdin访问了课程信息";
        emit send_log();
    }
    else if(item->text(column)=="作业信息")
    {
        ui->stackedWidget->setCurrentIndex(1);
        str_log="管理员amdin访问了作业信息";
        emit send_log();
    }
    else if(item->text(column)=="日志系统")
    {
        ui->stackedWidget->setCurrentIndex(2);
        str_log="管理员amdin访问了日志系统";
        emit send_log();
    }
}

void administerWindow::on_btn_search_2_clicked()
{

    if(ui->line_search_2->text().isEmpty())
    {
        return;
    }
    else
    {
        clear_homework_table();
        ui->homework_table->clearContents();
        int row = 0;
        for(int i = 0;i < total_course;i++)
        {
            int k = 0;
            int flag = 1;
            while(k < i)
            {
                if(item_c[k].get_course_name().compare(item_c[i].get_course_name())==0)
                {
                    flag = 0;
                }
                k++;
            }
            if(flag == 1)
            {
                int j =0;
                while(item_c[i].get_homework()[j].now!=-1)
                {
                    int colindex = 0;
                    if(item_c[i].get_homework()[j].now == 1 && item_c[i].get_course_name()==ui->line_search_2->text())
                    {
                        cur_course=i;
                        this->ui->homework_table->insertRow(row);
                        this->ui->homework_table->setItem(row,colindex++,new QTableWidgetItem(QString::number(row,10)));
                        this->ui->homework_table->setItem(row,colindex++,new QTableWidgetItem(item_c[i].get_course_name()));
                        this->ui->homework_table->setItem(row,colindex++,new QTableWidgetItem(item_c[i].get_homework()[j].workname));
                        if(item_c[i].get_homework()[j].flag == 1)
                        {
                            this->ui->homework_table->setItem(row,colindex++,new QTableWidgetItem("已交"));
                        }
                        else if(item_c[i].get_homework()[j].flag == 0)
                        {
                            this->ui->homework_table->setItem(row,colindex++,new QTableWidgetItem("未交"));
                        }
                        total_homework++;
                        row++;
                    }
                    j++;
                }
            }
        }
        if(row == 0)
        {
            QMessageBox::information(this,"提示","未找到对应的课程作业,可发布新作业");
            ui->btn_search_2->setEnabled(false);
            ui->btn_return_2->setEnabled(true);
            ui->btn_issue->setEnabled(true);
        }
        else if(row > 0)
        {
            ui->btn_search_2->setEnabled(false);
            ui->btn_return_2->setEnabled(true);
            ui->btn_issue->setEnabled(true);
        }
        str_log="管理员amdin查询了课程："+ui->line_search_2->text();
        emit send_log();
    }
}

void administerWindow::on_btn_return_2_clicked()
{
    ui->homework_table->clear();
    ui->line_search_2->clear();
    clear_homework_table();
    inithomeworktable();
    ui->btn_search_2->setEnabled(true);
    ui->btn_return_2->setEnabled(false);
    ui->btn_issue->setEnabled(false);
    str_log="管理员amdin退出了查询课程作业状态";
    emit send_log();
}

void administerWindow::on_btn_issue_clicked()
{
    if(ui->line_issue->text().isEmpty())
    {
        return;
    }
    else
    {
        QString str="../total_information/"+ui->line_search_2->text()+"_homeworklist.txt";
        QFile file(str);
        bool isOk=file.open(QIODevice::Append);
        if(isOk==true)
        {
            QTextStream out(&file);
            int i;
            for(i=0;item_c[cur_course].get_homework()[i].now!=-1;i++);
            item_c[cur_course].get_homework()[i].workname=ui->line_issue->text();
            item_c[cur_course].get_homework()[i].flag=0;
            item_c[cur_course].get_homework()[i].now=1;
            item_c[cur_course].get_homework()[i+1].now=-1;
            //total_homework++;
            out << ui->line_issue->text() << " " << 0 << "\n";
        }
        else
        {
            qDebug() << "课程作业文件打开失败";
        }
        file.close();
        str_log="管理员amdin发布了课程："+ui->line_search_2->text()+"的作业："+ui->line_issue->text();
        emit send_log();
        ui->line_issue->clear();
        inithomeworktable();
        emit issue_over();
        QMessageBox::information(this,tr("提示"),tr("发布成功！"));
    }
}
void administerWindow::on_btn_clear_clicked()
{
    QFile file("../total_information/log.txt");
    file.open(QIODevice::WriteOnly);
    file.close();
    emit is_clear();
    QMessageBox::information(this,tr("提示"),tr("清空完成！"));
    str_log="管理员amdin清空了日志";
    emit send_log();
}
