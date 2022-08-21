#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QPushButton>
#include<QTextStream>
#include<QIODevice>
#include<QFile>
#include<QDebug>
#include<QStringList>
#include<QString>
#include<QTableWidget>
#include<QMessageBox>
#include<QTableWidgetItem>
#include<QTextCodec>
#include<QTimer>
#include<QTime>
#include<QDate>
#include<QDateTime>
#include <QInputDialog>
#include "administorwindow.h"
#include "ui_administorwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    loginpage.show();
    read_time();
    course_sort_menu = new QMenu(this);
    act1 = new QAction("按课程名排序",this);
    act2 = new QAction("按校区排序",this);
    act3 = new QAction("按上课时间排序",this);
    //把action追加到menu
    course_sort_menu->addAction(act1);
    course_sort_menu->addAction(act2);
    course_sort_menu->addAction(act3);
    //把menu加载到btn_course_sort
    ui->btn_course_sort->setMenu(course_sort_menu);
    course_guide_menu = new QMenu(this);
    act4 = new QAction("按课程名导航",this);
    act5 = new QAction("按上课时间导航",this);
    act6 = new QAction("按地点导航",this);
    //把action追加到menu
    course_guide_menu->addAction(act4);
    course_guide_menu->addAction(act5);
    course_guide_menu->addAction(act6);
    //把menu加载到guide_btn
    ui->guide_btn->setMenu(course_guide_menu);
    ring->setMedia(QUrl::fromLocalFile("../total_information/ring.mp3"));
    clock_add_menu = new QMenu(this);
    act7 = new QAction("添加单次闹钟",this);
    act8 = new QAction("添加每周闹钟",this);
    act9 = new QAction("添加每天闹钟",this);
    clock_add_menu->addAction(act7);
    clock_add_menu->addAction(act8);
    clock_add_menu->addAction(act9);
    ui->btn_add_clock->setMenu(clock_add_menu);
    ui->stackedWidget->setCurrentIndex(0);
    read_path();
    ui->treeWidget->clear();
    ui->treeWidget->setColumnCount(1);
    QStringList item;
    item <<"课程辅助系统";
    QTreeWidgetItem *phead = new QTreeWidgetItem(ui->treeWidget,item);
    ui->treeWidget->addTopLevelItem(phead);
    item.clear();
    item<<"课程信息";
    QTreeWidgetItem *p1 = new QTreeWidgetItem(phead,item);
    item.clear();
    item<<"课外活动";
    QTreeWidgetItem *p2 = new QTreeWidgetItem(phead,item);
    item.clear();
    item<<"导航系统";
    QTreeWidgetItem *p3 = new QTreeWidgetItem(phead,item);
    item.clear();
    item<<"资料查询";
    QTreeWidgetItem *p4 = new QTreeWidgetItem(phead,item);
    item.clear();
    item<<"作业查询";
    QTreeWidgetItem *p5 = new QTreeWidgetItem(phead,item);
    item.clear();
    item<<"活动闹钟";
    QTreeWidgetItem *p6 = new QTreeWidgetItem(phead,item);
    phead->addChild(p1);
    phead->addChild(p2);
    phead->addChild(p3);
    phead->addChild(p4);
    phead->addChild(p5);
    phead->addChild(p6);
    ui->treeWidget->expandAll();
    ui->treeWidget->setCurrentItem(p1);
    read_all_information();
    initcousetable();
    initactivitytable();
    initcousedatatable();
    inithomeworktable();
    initactivity1table();
    initclocktable();
    connect(&loginpage,&page_login::sendStudentLogin,this,[=](){
        this->show();
        loginpage.hide();
        current_time->start(17);
        write_log("用户student登录了课程辅助系统");
        emit in_log();
        write_log("用户student访问了课程信息");
        emit in_log();
    });
    connect(ui->btn_exit,&QPushButton::clicked,this,[=](){
        this->hide();
        loginpage.show();
        current_time->stop();
        write_time();
        write_log("用户student登出了课程辅助系统");
        emit in_log();
    });
    connect(&loginpage,&page_login::sendAdminLogin,&adminwindow,[=](){
        adminwindow.show();
        loginpage.hide();
        current_time->start(17);
        write_log("管理员admin登录了课程管理系统");
        emit in_log();
        write_log("管理员admin访问了课程信息");
        emit in_log();
    });
    connect(&adminwindow,&administerWindow::is_closed,this,[=](){
        loginpage.show();
        current_time->stop();
        write_time();
        write_log("管理员admin登出了课程管理系统");
        emit in_log();
    });
    connect(current_time,&QTimer::timeout,this,[=](){
        timer_counter();
        QTime t(i_time.hour,i_time.minute,i_time.second);
        QDate d(i_time.year,i_time.month,i_time.day);
        QDateTime cur_time(d,t);
        QString str_time=cur_time.toString("yyyy-MM-dd hh:mm dddd");
        ui->label_time->setText(str_time);
        //clock_act();
    });
    connect(current_time,&QTimer::timeout,&adminwindow,[=](){
        timer_counter();
        QTime t(i_time.hour,i_time.minute,i_time.second);
        QDate d(i_time.year,i_time.month,i_time.day);
        QDateTime cur_time(d,t);
        QString str_time=cur_time.toString("yyyy-MM-dd hh:mm dddd");
        adminwindow.get_ui()->label_time->setText(str_time);
        });
    connect(&input_b,&input_activity::is_closed,this,[=](){
        item_a[a[choose]]=input_b.get_activity();
        update_act();
        QMessageBox::information(this,tr("提示"),tr("修改完成！"));
        ui->activity_table->clear();
        initactivitytable();
        ui->btn_sort_act->setEnabled(true);
        ui->btncancel_act->setEnabled(false);
        ui->btn_sort_act->setEnabled(true);
        ui->btndelete_act->setEnabled(false);
        ui->btnupdate_act->setEnabled(false);
        ui->act_search_line->clear();
        num = 0;
        write_log("用户student修改了"+input_b.get_activity().gettype() + "：" + input_b.get_activity().getname());
        emit in_log();
    });
    connect(&input_a,&input_activity::is_closed,this,[=](){
        item_a[total_activity]=input_a.get_activity();
        total_activity++;
        update_act();
        QMessageBox::information(this,tr("提示"),tr("添加完成！"));
        initactivitytable();
        write_log("用户student添加了"+input_a.get_activity().gettype() + "：" + input_a.get_activity().getname());
        emit in_log();
    });
    connect(adminwindow.get_ui()->btn_start,&QPushButton::clicked,this,[=](){
        current_time->start(17);
        write_log("管理员admin恢复了系统时间");
        emit in_log();
    });
    connect(adminwindow.get_ui()->btn_stop,&QPushButton::clicked,this,[=](){
        current_time->stop();
        write_log("管理员admin暂停了系统时间");
        emit in_log();
    });
    connect(adminwindow.get_ui()->btn_accelerate,&QPushButton::clicked,this,[=](){
        current_time->stop();
        current_time->start(3);
        write_log("管理员admin加速了系统时间");
        emit in_log();
    });
    connect(&adminwindow,&administerWindow::is_clear,this,[=](){
        update_log();
        adminwindow.get_ui()->log_list->clear();
    });
    connect(&adminwindow,&administerWindow::send_log,this,[=](){
        write_log(adminwindow.get_str());
        emit in_log();
    });
    connect(this,&MainWindow::in_log,this,&MainWindow::update_log);
    ///connect(adminwindow.ui)
    connect(act1,&QAction::triggered,this,[=](){
        sort1();
        write_log("用户student选择了按课程名排序");
        emit in_log();
    });
    connect(act2,&QAction::triggered,this,[=](){
        sort2();
        write_log("用户student选择了按上课地点排序");
        emit in_log();
    });
    connect(act3,&QAction::triggered,this,[=](){
        sort3();
        write_log("用户student选择了按上课时间排序");
        emit in_log();
    });
    connect(act4,&QAction::triggered,this,[=](){
        guide1();
        write_log("用户student选择了按课程名导航");
        emit in_log();
    });
    connect(act5,&QAction::triggered,this,[=](){
        guide2();
        write_log("用户student选择了按上课时间导航");
        emit in_log();
    });
    connect(act6,&QAction::triggered,this,[=](){
        guide3();
        write_log("用户student选择了按地点导航");
        emit in_log();
    });
    connect(act7,&QAction::triggered,this,&MainWindow::add_clock1);
    connect(act8,&QAction::triggered,this,&MainWindow::add_clock2);
    connect(act9,&QAction::triggered,this,&MainWindow::add_clock3);
    connect(ui->treeWidget,&QTreeWidget::currentItemChanged,this,&MainWindow::on_treewidgetitem_changed);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::read_all_information()
{
    total_course=0;
    total_activity=0;
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
    QFile file2("../total_information/activity_information.txt");
    isOk = file2.open(QIODevice::ReadOnly);
    if(isOk == true)
    {
        QTextStream in2(&file2);
        while(!read_activity(in2,item_a[total_activity]).atEnd())
        {
            total_activity++;
        }
     }
    else
    {
         qDebug() << "总活动信息文件打开失败";
    }

     file2.close();
     QFile file3("../total_information/schoolcar.txt");
     isOk =file3.open(QIODevice::ReadOnly);
     if(isOk==true)
     {
         QTextStream in3(&file3);
         int i=0;
         while(!in3.atEnd())
         {
             in3 >> bushour[i] >> busmin[i];
             i++;
         }
     }
     else
     {
         qDebug() << "班车信息文件打开失败";
     }
     file3.close();
}

void MainWindow:: on_treewidgetitem_changed()
{
    QTreeWidgetItem *curitem = ui->treeWidget->currentItem();
    if(curitem->text(0) == "课程信息")
    {
        ui->stackedWidget->setCurrentIndex(0);
        write_log("用户student访问了课程信息");
        emit in_log();
    }
    else if(curitem->text(0) == "课外活动")
    {
        ui->stackedWidget->setCurrentIndex(1);
        detect_collision(item_c,item_a);
        write_log("用户student访问了课外活动");
        emit in_log();
    }
    else if(curitem->text(0) == "导航系统")
    {
        ui->stackedWidget->setCurrentIndex(2);
        write_log("用户student访问了导航系统");
        emit in_log();
    }
    else if(curitem->text(0) == "资料查询")
    {
        ui->stackedWidget->setCurrentIndex(3);
        write_log("用户student访问了资料查询");
        emit in_log();
    }
    else if(curitem->text(0) == "作业查询")
    {
        ui->stackedWidget->setCurrentIndex(4);
        write_log("用户student访问了作业查询。");
        emit in_log();
    }
    else if(curitem->text(0) == "活动闹钟")
    {
        ui->stackedWidget->setCurrentIndex(5);
        write_log("用户student访问了活动闹钟");
        emit in_log();
    }
}

void MainWindow::initcousetable() //课程信息表格初始化
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
            << QObject::tr("考试地点")
            << QObject::tr("已交作业")
            << QObject::tr("待交作业")
            << QObject::tr("资料");
    this->ui->course_table->setColumnCount(headers.size()); //设置列数
    this->ui->course_table->setHorizontalHeaderLabels(headers);//列名称
    this->ui->course_table->setRowCount(total_course);//设置行数
    this->ui->course_table->setAlternatingRowColors(true);//行间隔色
    this->ui->course_table->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);//自动列宽
    this->ui->course_table->installEventFilter(this);
    for(int row = 0;row < total_course;row++)
    {
        int colindex = 0;
        this->ui->course_table->setItem(row,colindex++,new QTableWidgetItem(QString::number(row,10)));
        this->ui->course_table->setItem(row,colindex++,new QTableWidgetItem(item_c[row].get_course_name()));
        this->ui->course_table->setItem(row,colindex++,new QTableWidgetItem(item_c[row].get_teacher_name()));
        QString str = QString("%1 %2:%3-%4:%5").arg(item_c[row].get_start_time().day_of_week)
                                             .arg(item_c[row].get_start_time().hour)
                                               .arg(item_c[row].get_start_time().minute)
                                               .arg(item_c[row].get_end_time().hour)
                                               .arg(item_c[row].get_end_time().minute);
        this->ui->course_table->setItem(row,colindex++,new QTableWidgetItem(str));
        str = QString("%1 %2").arg(item_c[row].get_course_location().campus).arg(item_c[row].get_course_location().position);
        this->ui->course_table->setItem(row,colindex++,new QTableWidgetItem(str));
        this->ui->course_table->setItem(row,colindex++,new QTableWidgetItem(item_c[row].get_course_group()));
        str = QString("%1/%2").arg(item_c[row].get_cur_section()).arg(item_c[row].get_total_section());
        this->ui->course_table->setItem(row,colindex++,new QTableWidgetItem(str));
        str = QString("第%1周 %2 %3:%4-%5:%6").arg(item_c[row].get_exam_start_time().week)
                                              .arg(item_c[row].get_exam_start_time().day_of_week)
                                                     .arg(item_c[row].get_exam_start_time().hour)
                                                       .arg(item_c[row].get_exam_start_time().minute)
                                                       .arg(item_c[row].get_exam_end_time().hour)
                                                       .arg(item_c[row].get_exam_end_time().minute);
        this->ui->course_table->setItem(row,colindex++,new QTableWidgetItem(str));
        str = QString("%1 %2").arg(item_c[row].get_exam_loc().campus).arg(item_c[row].get_exam_loc().position);
        this->ui->course_table->setItem(row,colindex++,new QTableWidgetItem(str));
        str = " ";
        int i = 0;
        while(item_c[row].get_homework()[i].flag!=-1)
        {
            if(item_c[row].get_homework()[i].flag == 1 && item_c[row].get_homework()[i].now == 1)
            {
                str += item_c[row].get_homework()[i].workname;
                str += " ";
            }
            i++;
        }
        this->ui->course_table->setItem(row,colindex++,new QTableWidgetItem(str));
        str = " ";
        i = 0;
        while(item_c[row].get_homework()[i].flag!=-1)
        {
            if(item_c[row].get_homework()[i].flag == 0 && item_c[row].get_homework()[i].now == 1)
            {
                str += item_c[row].get_homework()[i].workname;
                str += " ";
            }
            i++;
        }
        this->ui->course_table->setItem(row,colindex++,new QTableWidgetItem(str));
        str = " ";
        i = 0;
        while(item_c[row].get_coursedata()[i].flag!=-1)
        {
            if(item_c[row].get_coursedata()[i].flag == 1 ){
                str += item_c[row].get_coursedata()[i].dataname;
                str += " ";
            }
            i++;
        }
        this->ui->course_table->setItem(row,colindex++,new QTableWidgetItem(str));
    }
}


void MainWindow::initactivitytable() //课外活动信息表格初始化
{
    QStringList headers;
    headers << QObject::tr("序号")
            << QObject::tr("活动名称")
            << QObject::tr("活动类型")
            << QObject::tr("活动时间")
            << QObject::tr("活动地点");
    this->ui->activity_table->setColumnCount(headers.size()); //设置列数
    this->ui->activity_table->setHorizontalHeaderLabels(headers);//列名称
    this->ui->activity_table->setRowCount(total_activity);//设置行数
    this->ui->activity_table->setAlternatingRowColors(true);//行间隔色  
    this->ui->activity_table->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);//自动列宽
    this->ui->activity_table->installEventFilter(this);
    for(int row = 0;row < total_activity;row++)
    {
        int colindex = 0;
        this->ui->activity_table->setItem(row,colindex++,new QTableWidgetItem(QString::number(row,10)));
        this->ui->activity_table->setItem(row,colindex++,new QTableWidgetItem(item_a[row].getname()));
        this->ui->activity_table->setItem(row,colindex++,new QTableWidgetItem(item_a[row].gettype()));
        QString str = QString("%6-%7-%8 %1 %2:%3-%4:%5").arg(item_a[row].get_start_time().day_of_week)
                                                        .arg(item_a[row].get_start_time().hour)
                                                        .arg(item_a[row].get_start_time().minute)
                                                        .arg(item_a[row].get_end_time().hour)
                                                        .arg(item_a[row].get_end_time().minute)
                                                        .arg(item_a[row].get_start_time().year)
                                                        .arg(item_a[row].get_start_time().month)
                                                        .arg(item_a[row].get_start_time().day);
        this->ui->activity_table->setItem(row,colindex++,new QTableWidgetItem(str));
        if(item_a[row].get_activity_loc().campus == "无")
        {
            this->ui->activity_table->setItem(row,colindex++,new QTableWidgetItem(item_a[row].get_activity_loc().position));
        }
        else
        {
        str = QString("%1 %2").arg(item_a[row].get_activity_loc().campus).arg(item_a[row].get_activity_loc().position);
        this->ui->activity_table->setItem(row,colindex++,new QTableWidgetItem(str));
        }
    }

}

void MainWindow::initcousedatatable() //课程资料信息表格初始化
{
    QStringList headers;
    headers << QObject::tr("序号")
            << QObject::tr("课程名称")
            << QObject::tr("资料名称")
            << QObject::tr("上传时间");
    this->ui->coursedata_table->setColumnCount(headers.size()); //设置列数
    this->ui->coursedata_table->setHorizontalHeaderLabels(headers);//列名称
    this->ui->coursedata_table->setAlternatingRowColors(true);//行间隔色
    this->ui->coursedata_table->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);//自动列宽
    this->ui->coursedata_table->installEventFilter(this);
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
            while(item_c[i].get_coursedata()[j].flag!=-1)
            {
                int colindex = 0;
                if(item_c[i].get_coursedata()[j].flag == 1)
                {
                    this->ui->coursedata_table->insertRow(row);
                    this->ui->coursedata_table->setItem(row,colindex++,new QTableWidgetItem(QString::number(row,10)));
                    this->ui->coursedata_table->setItem(row,colindex++,new QTableWidgetItem(item_c[i].get_course_name()));
                    this->ui->coursedata_table->setItem(row,colindex++,new QTableWidgetItem(item_c[i].get_coursedata()[j].dataname));
                    QString str = " ";
                    str = str  + item_c[i].get_coursedata()[j].up_date.year +".";
                    str = str +item_c[i].get_coursedata()[j].up_date.month + ".";
                    str = str +item_c[i].get_coursedata()[j].up_date.day + " ";
                    str = str +item_c[i].get_coursedata()[j].up_date.hour + ":";
                    str = str +item_c[i].get_coursedata()[j].up_date.minute;
                    this->ui->coursedata_table->setItem(row,colindex++,new QTableWidgetItem(str));
                    row++;
                    total_data++;
                }
                j++;
            }
        }
    }
}
void MainWindow::inithomeworktable() //课程作业信息表格初始化
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
void MainWindow::clear_homework_table()
{
    for(int i=total_homework;i>=0;i--)
    {
        ui->homework_table->removeRow(i);
        total_homework--;
    }
}
void MainWindow::clear_data_table()
{
    for(int i=total_data;i>=0;i--)
    {
        ui->coursedata_table->removeRow(i);
        total_data--;
    }
}
void MainWindow::on_btn_course_sort_clicked()
{
    course_sort_menu->exec(QPoint(QCursor::pos().x(),QCursor::pos().y()));
    //打开按钮的菜单
    write_log("用户student进入了课程排序状态");
    emit in_log();
}

//将课程信息按课程名排序槽函数
void MainWindow::sort1()
{
    quicksort(item_c,0,total_course - 1,1);
    ui->course_table->clear();
    initcousetable();
}

//将课程信息按上课地点所在校区排序槽函数
void MainWindow::sort2()
{
    quicksort(item_c,0,total_course - 1,2);
    ui->course_table->clear();
    initcousetable();
}

void MainWindow::sort3()
{
    quicksort(item_c,0,total_course-1,3);
    ui->course_table->clear();
    initcousetable();
}

void MainWindow::guide1()
{
    if(ui->cur_pos_line->text().isEmpty()||ui->des_pos_line->text().isEmpty())
    {
        return;
    }
    else
    {
        int start = ui->cur_pos_line->text().toInt();
        int end=0;
        int flag =0;
        int x=0;
        for(int i=0;i<total_course;i++)
        {
            QTime t(i_time.hour,i_time.minute,i_time.second);
            QDate d(i_time.year,i_time.month,i_time.day);
            QDateTime cur_time(d,t);
            QString str =cur_time.toString("dddd");
            qDebug() << str;
            int temp =day_of_week_transfer(str);
            if(item_c[i].get_course_name() == ui->des_pos_line->text())
            {
                flag =1;
                if(end==0||((day_of_week_transfer(item_c[x].get_start_time().day_of_week)<temp&&temp<day_of_week_transfer(item_c[i].get_start_time().day_of_week))
                   ||(day_of_week_transfer(item_c[i].get_start_time().day_of_week)<day_of_week_transfer(item_c[x].get_start_time().day_of_week)&&day_of_week_transfer(item_c[x].get_start_time().day_of_week)<temp)))
                {
                    end =item_c[i].get_course_location().n;
                    x=i;
                }
            }
        }
        if(flag ==0)
        {
              QMessageBox::information(this,"提示","未找到对应的课程");
        }
        else
        {
              search_shortest_path(start,end);
        }
        ui->cur_pos_line->clear();
        ui->des_pos_line->clear();
    }
}

void MainWindow::guide2()
{
    if(ui->cur_pos_line->text().isEmpty()||ui->des_pos_line->text().isEmpty())
    {
        return;
    }
    else
    {
        int start = ui->cur_pos_line->text().toInt();
        QStringList strl = ui->des_pos_line->text().split(" ");

        int flag =0;
        for(int i=0;i<total_course;i++)
        {

            if(item_c[i].get_start_time().day_of_week==strl.at(0)&&item_c[i].get_start_time().hour==strl.at(1)
               &&item_c[i].get_start_time().minute==strl.at(2))
            {
                flag =1;
                search_shortest_path(start,item_c[i].get_course_location().n);
            }
        }
        if(flag ==0)
        {
              QMessageBox::information(this,"提示","未找到对应的课程");
        }
    }
    ui->cur_pos_line->clear();
    ui->des_pos_line->clear();
}

void MainWindow::guide3()
{
    if(ui->cur_pos_line->text().isEmpty()||ui->des_pos_line->text().isEmpty())
    {
        return;
    }
    else
    {
        int start = ui->cur_pos_line->text().toInt();
        for (int i = 1; i <= total_path; i++)
        {
            if(b[i].name == ui->des_pos_line->text())
            {
                int goal = b[i].n;
                search_shortest_path(start,goal);
            }
        }
    }
    ui->cur_pos_line->clear();
    ui->des_pos_line->clear();
}

void MainWindow::on_btn_search_clicked()
{
    if(ui->search_line->text().isEmpty())
    {
        return;
    }
    else
    {
        ui->course_table->clearContents();
        int row = 0;
        for(int i = 0;i < total_course;i++)
        {
            if(item_c[i].get_course_name() == ui->search_line->text())
            {
                int colindex = 0;
                this->ui->course_table->setItem(row,colindex++,new QTableWidgetItem(QString::number(row+1,10)));
                this->ui->course_table->setItem(row,colindex++,new QTableWidgetItem(item_c[i].get_course_name()));
                this->ui->course_table->setItem(row,colindex++,new QTableWidgetItem(item_c[i].get_teacher_name()));
                QString str = QString("%1 %2:%3-%4:%5").arg(item_c[i].get_start_time().day_of_week)
                                                       .arg(item_c[i].get_start_time().hour)
                                                       .arg(item_c[i].get_start_time().minute)
                                                       .arg(item_c[i].get_end_time().hour)
                                                       .arg(item_c[i].get_end_time().minute);
                this->ui->course_table->setItem(row,colindex++,new QTableWidgetItem(str));
                str = QString("%1 %2").arg(item_c[i].get_course_location().campus).arg(item_c[i].get_course_location().position);
                this->ui->course_table->setItem(row,colindex++,new QTableWidgetItem(str));
                this->ui->course_table->setItem(row,colindex++,new QTableWidgetItem(item_c[i].get_course_group()));
                str = QString("%1/%2").arg(item_c[i].get_cur_section()).arg(item_c[i].get_total_section());
                this->ui->course_table->setItem(row,colindex++,new QTableWidgetItem(str));
                str = QString("第%1周 %2 %3:%4-%5:%6").arg(item_c[i].get_exam_start_time().week)
                                                      .arg(item_c[i].get_exam_start_time().day_of_week)
                                                      .arg(item_c[i].get_exam_start_time().hour)
                                                      .arg(item_c[i].get_exam_start_time().minute)
                                                      .arg(item_c[i].get_exam_end_time().hour)
                                                      .arg(item_c[i].get_exam_end_time().minute);
                this->ui->course_table->setItem(row,colindex++,new QTableWidgetItem(str));
                str = QString("%1 %2").arg(item_c[row].get_exam_loc().campus).arg(item_c[i].get_exam_loc().position);
                this->ui->course_table->setItem(row,colindex++,new QTableWidgetItem(str));
                row++;
            }
        }
        if(row == 0)
        {
            QMessageBox::information(this,"提示","该课程不存在");
            initcousetable();
        }
        else if(row > 0)
        {            
            ui->btn_course_sort->setEnabled(false);
            ui->btn_cancel_search->setEnabled(true);
        }
        write_log("用户student查询了课程"+ui->search_line->text());
        emit in_log();
    }
}
void MainWindow::on_btn_cancel_search_clicked()
{
    ui->course_table->clear();
    initcousetable();
    ui->btn_cancel_search->setEnabled(false);
    ui->btn_course_sort->setEnabled(true);
    write_log("用户student退出了查询课程状态");
    emit in_log();
}

void MainWindow::on_btnsearch_act_clicked()
{
    if(ui->act_search_line->text().isEmpty())
    {
        return;
    }
    else
    {
        ui->activity_table->clearContents();
        int row = 0;
        for(int i = 0;i < total_activity;i++)
        {
            if(item_a[i].gettype() == ui->act_search_line->text())
            {
                a[num]=i;
                num++;
                int colindex = 0;
                this->ui->activity_table->setItem(row,colindex++,new QTableWidgetItem(QString::number(row+1,10)));
                this->ui->activity_table->setItem(row,colindex++,new QTableWidgetItem(item_a[i].getname()));
                this->ui->activity_table->setItem(row,colindex++,new QTableWidgetItem(item_a[i].gettype()));
                QString str = QString("%1 %2:%3-%4:%5").arg(item_a[i].get_start_time().day_of_week)
                                                     .arg(item_a[i].get_start_time().hour)
                                                       .arg(item_a[i].get_start_time().minute)
                                                       .arg(item_a[i].get_end_time().hour)
                                                       .arg(item_a[i].get_end_time().minute);
                this->ui->activity_table->setItem(row,colindex++,new QTableWidgetItem(str));
                if(item_a[i].get_activity_loc().campus == "无"){
                    this->ui->activity_table->setItem(row,colindex++,new QTableWidgetItem(item_a[i].get_activity_loc().position));
                }
                else{
                str = QString("%1 %2").arg(item_a[i].get_activity_loc().campus).arg(item_a[i].get_activity_loc().position);
                this->ui->activity_table->setItem(row,colindex++,new QTableWidgetItem(str));
                }
                row++;
            }
        }
        if(row == 0)
        {
             QMessageBox::information(this,"提示","未找到对应的活动");
             initactivitytable();
        }
        else if(row > 0)
        {
            ui->btn_sort_act->setEnabled(false);
            ui->btncancel_act->setEnabled(true);
            ui->btn_sort_act->setEnabled(false);
            ui->btndelete_act->setEnabled(true);
            ui->btnupdate_act->setEnabled(true);
            num = 0;
        }
        write_log("用户student查询了"+ui->act_search_line->text());
        emit in_log();
    }
}

void MainWindow::on_btncancel_act_clicked()
{
    ui->activity_table->clear();
    initactivitytable();
    ui->btn_sort_act->setEnabled(true);
    ui->btncancel_act->setEnabled(false);
    ui->btn_sort_act->setEnabled(true);
    ui->btndelete_act->setEnabled(false);
    ui->btnupdate_act->setEnabled(false);
    ui->act_search_line->clear();
    num = 0;
    write_log("用户student退出了查询活动状态");
    emit in_log();
}


int MainWindow::compare_time(vtime a,vtime b)
{
    int a_hour = a.hour.toInt();
    int b_hour = b.hour.toInt();
    int a_minute = a.minute.toInt();
    int b_minute = b.minute.toInt();//将a,b的时间转换为整型
    if(a_hour > b_hour)
    {
        return 1;
    }
    else if(a_hour == b_hour)
    {
        if(a_minute > b_minute)
        {
            return 1;
        }
        else if(a_minute == b_minute)
        {
            return 0;
        }
        else
        {
            return -1;
        }
    }
    else
    {
        return -1;
    }
} //比较a和b的时间，若a>b，则返回1，若a=b，则返回0，若a<b，则返回-1

int MainWindow::activity_collision_detect(vtime a_start,vtime a_end,vtime b_start,vtime b_end)//对活动与活动或活动与课程进行时间冲突检查,若出现冲突则返回1，否则返回0
{
    if((a_start.week == b_start.week&&a_start.day_of_week==b_start.day_of_week) || (a_start.week == "-1"&&a_start.day_of_week==b_start.day_of_week)) //判断两个活动的时间是否在同一周，若a为课程则默认为同一周
    {
        if(compare_time(a_start,b_start) > 0)
        {
            if(compare_time(b_end,a_start) <= 0)
            {
                return 0;
            }
            else
            {
                return 1;
            }
        }//a的开始时间晚于b，b的结束时间早于或等于a的开始时间时返回0，不冲突，其余情况返回1，发生冲突
        else if(compare_time(a_start,b_start) == 0)
        {
            return 1;
        }//a，b的开始时间相同，发生冲突，返回1
        else
        {
            if(compare_time(a_end,b_start) <= 0)
            {
                return 0;
            }
            else
            {
                return 1;
            }
        }//a的开始时间早于b，a的结束时间早于或等于b的开始时间时返回0，不冲突，其余情况返回1，发生冲突
    }
    return 0;
}

void MainWindow::detect_collision(course *item_c,activity *item_a)
{
    QString str;
    QString hh = "\n";
    for(int i = 0;i < total_activity;i++)
    {
        for(int t = i - 1;t >= 0;t--)
        {
            if(activity_collision_detect(item_a[i].get_start_time(),item_a[i].get_end_time(),item_a[t].get_start_time(),item_a[t].get_end_time()))
            {
                QString temp = QString("%1: %2 与 %3: %4 时间冲突").arg(item_a[i].gettype())
                                                                   .arg(item_a[i].getname())
                                                                   .arg(item_a[t].gettype())
                                                                   .arg(item_a[t].getname());
                str+=temp;
                str.append(hh);
            }
        }
        for(int j = 0;j < total_course;j++)
        {
            if(activity_collision_detect(item_c[j].get_start_time(),item_c[j].get_end_time(),item_a[i].get_start_time(),item_a[i].get_end_time()))
            {
                QString temp = QString("课程: %1 与 %2: %3 时间冲突").arg(item_c[j].get_course_name())
                                                                   .arg(item_a[i].gettype())
                                                                   .arg(item_a[i].getname());
                str+=temp;
                str.append(hh);
            }
        }//将所有活动与所有课程进行遍历，检测冲突情况，并输出
    }//将所有活动与所有课程进行遍历，检测冲突情况，并输出 测每个活动是否与其他活动或者所有课程有冲突，输出冲突情况
    QMessageBox::information(this,"提示",str);
}

void MainWindow::read_path(void) //从文件中读取路
{
    QFile file("../total_information/map.txt");
    bool isOk = file.open(QIODevice::ReadOnly);
    if(isOk == true)
    {
        QTextStream f(&file);
        f >> total_building;
        for (int i = 1; i <= total_building; i++)
        {
            f >> b[i].name  >>b[i].area >> b[i].n ;
        }
        f >> total_path;
        for (int i = 1; i <= total_path; i++)
        {
            f >> p[i].area >> p[i].a >> p[i].b >> p[i].kind >> p[i].length >> p[i].crowd;
        }
    }
    else
    {
        qDebug() << "地图文件打开失败";
    }
    file.close();
}

void MainWindow::dijkstra(int vis[], int dis[], int start, int goal)  //迪杰斯特拉算法
{
    for (int i = 1; i <= total_building; i++)
    {
        dis[i] = cost[start][i];
        if (dis[i] < 9999)pre_p[i] = start;
    }
    vis[start] = 1;
    for (int i = 1; vis[goal] == 0 && i <= total_building ; i++)
    {
        int node = -1;
        for (int j = 1; j <= total_building ; j++)
        {

            if (!vis[j] && (node == -1 || dis[j] < dis[node]))
            {
                node = j;
            }
        }
        vis[node] = 1;
        for (int j = 1; j <= total_building ; j++)
        {

            if (!vis[j] && (dis[j] > cost[node][j] + dis[node]))
            {
                dis[j] = cost[node][j] + dis[node];
                pre_p[j] = node;
            }

        }
    }
}

QString MainWindow::output_path(int start, int goal)   //输出路径
{
    int pre[201] = { 0 };
    int temp = goal;
    int i = 1;
    while (temp != start)
    {
        pre[i] = temp;
        temp = pre_p[temp];
        i++;
    }
    pre[i] = start;
    QString str = NULL;
    for (int j = i; j > 1; j--)
    {
        QString str1 = QString::number(pre[j]);
        str = str + str1;
        str = str + " -> ";
    }
    QString str2 = QString::number(pre[1]);
    str = str + str2;
    return str;
}

void MainWindow::search_shortest_path(int start, int goal)
{
    int vis[201];
    int dis[201];
    QString hh = "\n";
    for (int i = 1; i <= 200; i++)
    {
        vis[i] = 0;
        pre_p[i] = 0;
        for (int j = 1; j <= 200; j++)
        {
            if (i == j)cost[i][j] = 0;
            else cost[i][j] = 9999;
        }
    }
    for (int i = 1; i < total_path; i++)
    {

        cost[p[i].a][p[i].b] = p[i].length;
        cost[p[i].b][p[i].a] = p[i].length;
    }
    QString str1 = "最短路径策略：";
    if (b[start].area == b[goal].area)
    {
        dijkstra(vis, dis, start, goal);
        QString str2 = output_path(start, goal);
        str1 = str1 +str2;
        str1.append(hh);
        str1 = str1 + "距离为";
        QString str5 = QString::number(dis[goal]);
        str1 = str1 + str5;
        str1 = str1 + "米";
    }
    else
    {
        if (b[start].area == 0)
        {
            dijkstra(vis, dis, start, 1);
            QString str2 = output_path(start,1);
            str1 = str1 +str2;
            str1.append(hh);
            str1 = str1 + "距离为";
            QString str5 = QString::number(dis[1]);
            str1 = str1 + str5;
            str1 = str1 + "米";
            str1.append(hh);
            dijkstra(vis, dis, 76, goal);
            QString str3 =output_path(76,goal);
            str1 = str1 +str3;
            str1.append(hh);
            str1 = str1 + "距离为";
            QString str6 = QString::number(dis[goal]);
            str1 = str1 + str6;
            str1 = str1 + "米";
        }
        else if (b[start].area == 1)
        {
            dijkstra(vis, dis, start, 76);
            QString str2 = output_path(start, 76);
            str1 = str1 +str2;
            str1.append(hh);
            str1 = str1 + "距离为";
            QString str5 = QString::number(dis[76]);
            str1 = str1 + str5;
            str1 = str1 + "米";
            str1.append(hh);
            dijkstra(vis, dis, 1, goal);
            QString str3 = output_path(1, goal);
            str1 = str1 +str3;
            str1.append(hh);
            str1 = str1 + "距离为";
            QString str6 = QString::number(dis[goal]);
            str1 = str1 + str6;
            str1 = str1 + "米";
        }
    }
    str1.append(hh);
    str1 = str1 +" 最短时间策略：";
    for(int i = 1;i <= 200;i++)
    {
        vis[i] = 0;
        pre_p[i] = 0;
        for(int j=1;j<=200;j++)
        {
            if(i==j)
                cost[i][j] = 0;
            else
                cost[i][j] = 9999;
        }
    }
    for(int i = 1; i <= total_path; i++)
    {

        cost[p[i].a][p[i].b] = (1/p[i].crowd) * p[i].length;
        cost[p[i].b][p[i].a] = (1/p[i].crowd) * p[i].length;
    }
    if (b[start].area == b[goal].area)
    {
        dijkstra(vis, dis, start, goal);
        QString str2 = output_path(start, goal);
        str1 = str1 +str2;
        str1.append(hh);
        str1 = str1 + "花费时间为";
        QString str5 = QString::number(dis[goal]/60);
        str1 = str1 + str5;
        str1 = str1 + "分";
    }
    else
    {
        if (b[start].area == 0)
        {
            dijkstra(vis, dis, start, 1);
            QString str2 = output_path(start, 1);
            str1 = str1 +str2;
            str1.append(hh);
            str1 = str1 + "花费时间为";
            QString str5 = QString::number(dis[1]/60);
            str1 = str1 + str5;
            str1 = str1 + "分";
            str1.append(hh);
            dijkstra(vis, dis, 76, goal);
            QString str3 = output_path(76, goal);
            str1 = str1 +str3;
            str1.append(hh);
            str1 = str1 + "花费时间为";
            QString str6 = QString::number(dis[goal]/60);
            str1 = str1 + str6;
            str1 = str1 + "分";
        }
        else if (b[start].area == 1)
        {
            dijkstra(vis, dis, start, 76);
            QString str2 = output_path(start, 76);
            str1 = str1 +str2;
            str1.append(hh);
            str1 = str1 + "花费时间为";
            QString str5 = QString::number(dis[76]/60);
            str1 = str1 + str5;
            str1 = str1 + "分";
            str1.append(hh);
            dijkstra(vis, dis, 1, goal);
            QString str3 = output_path(1, goal);
            str1 = str1 +str3;
            str1.append(hh);
            str1 = str1 + "花费时间为";
            QString str6 = QString::number(dis[goal]/60);
            str1 = str1 + str6;
            str1 = str1 + "分";
        }
        int ccost=90;
        int fflag = -1;
        for(int kk = 0 ; kk < 5 ; kk++)
        {
            if(i_time.hour <= bushour[kk])
            {
                int tempp = 60*(bushour[kk]-i_time.hour)+(busmin[kk] - i_time.minute)+40;
                if(tempp < ccost)
                {
                    fflag = kk;
                    ccost = tempp;
                }
            }
        }
        str1.append(hh);
        if(fflag == -1)str1 = str1 + "建议坐公交前往，需要90分";
        else
        {
            str1 = str1 + "建议乘坐校车前往，发车时间"+QString::number(bushour[fflag],10)+"时"+QString::number(busmin[fflag],10)+"分\n";
            str1 = str1 +"需要"+QString::number(ccost,10)+"分";
        }
    }
    str1.append(hh);
    str1 = str1 +" 骑车时：";
    for(int i = 1;i <= 200;i++)
    {
        vis[i] = 0;
        pre_p[i] = 0;
        for(int j=1;j<=200;j++)
        {
            if(i==j)
                cost[i][j] = 0;
            else
                cost[i][j] = 9999;
        }
    }
    for(int i = 1; i <= total_path; i++)
    {
        if(p[i].kind == 1)
        cost[p[i].a][p[i].b] = (1/p[i].crowd) * p[i].length;
        cost[p[i].b][p[i].a] = (1/p[i].crowd) * p[i].length;
    }
    if (b[start].area == b[goal].area)
    {
        dijkstra(vis, dis, start, goal);
        QString str2 = output_path(start, goal);
        str1 = str1 +str2;
        str1.append(hh);
        str1 = str1 + "花费时间为";
        QString str5 = QString::number(dis[goal]/180);
        str1 = str1 + str5;
        str1 = str1 + "分";
    }
    else
    {
        if (b[start].area == 0)
        {
            dijkstra(vis, dis, start, 1);
            QString str2 = output_path(start, 1);
            str1 = str1 +str2;
            str1.append(hh);
            str1 = str1 + "花费时间为";
            QString str5 = QString::number(dis[1]/180);
            str1 = str1 + str5;
            str1 = str1 + "分";
            str1.append(hh);
            dijkstra(vis, dis, 76, goal);
            QString str3 = output_path(76, goal);
            str1 = str1 +str3;
            str1.append(hh);
            str1 = str1 + "花费时间为";
            QString str6 = QString::number(dis[goal]/180);
            str1 = str1 + str6;
            str1 = str1 + "分";
        }
        else if (b[start].area == 1)
        {
            dijkstra(vis, dis, start, 76);
            QString str2 = output_path(start, 76);
            str1 = str1 +str2;
            str1.append(hh);
            str1 = str1 + "花费时间为";
            QString str5 = QString::number(dis[76]/180);
            str1 = str1 + str5;
            str1 = str1 + "分";
            str1.append(hh);
            dijkstra(vis, dis, 1, goal);
            QString str3 = output_path(1, goal);
            str1 = str1 +str3;
            str1.append(hh);
            str1 = str1 + "花费时间为";
            QString str6 = QString::number(dis[goal]/180);
            str1 = str1 + str6;
            str1 = str1 + "分";
        }
    }
    ui->guide_lab->setText(str1);
}
void MainWindow::on_map_btn_clicked()
{
    QPixmap pixmap(ic.NextImage());
    ui->map_lab->setPixmap(pixmap);
    ui->map_lab->show();
}

void MainWindow::on_btn_search_2_clicked()
{
    if(ui->search_line_2->text().isEmpty())
    {
        return;
    }
    else
    {
        clear_data_table();
        ui->coursedata_table->clearContents();
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
                while(item_c[i].get_coursedata()[j].flag!=-1 && item_c[i].get_course_name() == ui->search_line_2->text())
                {
                    int colindex = 0;
                    if(item_c[i].get_coursedata()[j].flag == 1)
                    {
                        this->ui->coursedata_table->insertRow(row);
                        this->ui->coursedata_table->setItem(row,colindex++,new QTableWidgetItem(QString::number(row,10)));
                        this->ui->coursedata_table->setItem(row,colindex++,new QTableWidgetItem(item_c[i].get_course_name()));
                        this->ui->coursedata_table->setItem(row,colindex++,new QTableWidgetItem(item_c[i].get_coursedata()[j].dataname));
                        QString str = " ";
                        str = str  + item_c[i].get_coursedata()[j].up_date.year +".";
                        str = str +item_c[i].get_coursedata()[j].up_date.month + ".";
                        str = str +item_c[i].get_coursedata()[j].up_date.day + " ";
                        str = str +item_c[i].get_coursedata()[j].up_date.hour + ":";
                        str = str +item_c[i].get_coursedata()[j].up_date.minute;
                        this->ui->coursedata_table->setItem(row,colindex++,new QTableWidgetItem(str));
                        total_data++;
                        row++;
                    }
                    j++;
                }
            }
        }
        if(row == 0)
        {
             QMessageBox::information(this,"提示","未找到对应的课程资料");
             initcousedatatable();
        }
        else if(row > 0)
        {
            ui->btn_search_2->setEnabled(false);
            ui->btn_cancel_search_2->setEnabled(true);
            ui->btn_upload->setEnabled(true);
        }
        write_log("用户student查询了课程："+ui->search_line_2->text()+"的课程资料");
        emit in_log();
    }
}

void MainWindow::on_btn_cancel_search_2_clicked()
{
    ui->coursedata_table->clear();
    clear_data_table();
    initcousedatatable();
    ui->search_line_2->clear();
    ui->btn_search_2->setEnabled(true);
    ui->btn_cancel_search_2->setEnabled(false);
    ui->btn_upload->setEnabled(false);
    write_log("用户student退出了查询课程资料状态");
    emit in_log();
}

void MainWindow::on_btn_search_3_clicked()
{
    if(ui->search_line_3->text().isEmpty())
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
                    if(item_c[i].get_homework()[j].now == 1 && item_c[i].get_course_name()==ui->search_line_3->text())
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
        if(row == 0)
        {
             QMessageBox::information(this,"提示","未找到对应的课程作业");
             inithomeworktable();
        }
        else if(row > 0)
        {
            ui->btn_search_3->setEnabled(false);
            ui->btn_cancel_search_3->setEnabled(true);
            ui->btn_upload_2->setEnabled(true);
        }
        write_log("用户student查询了课程："+ui->search_line_3->text()+"的作业");
        emit in_log();
    }
}

void MainWindow::on_btn_cancel_search_3_clicked()
{
    ui->homework_table->clear();
    clear_homework_table();
    inithomeworktable();
    ui->search_line_3->clear();
    ui->btn_search_3->setEnabled(true);
    ui->btn_cancel_search_3->setEnabled(false);
    ui->btn_upload_2->setEnabled(false);
    write_log("用户student退出了查询课程作业状态");
    emit in_log();
}
int MainWindow::strlength(char *a){      //计算字符数组的长度
    int n = 0;
    char *b = a;
    while(*b != '\0'){
        n++;
        b++;
    }
    return n;
}
void MainWindow::move(char *a,char *b, long n){  //拷贝字符数组，用来实现哈夫曼编码
    a = a + n - 1;
    b = b + n - 1;
    while(n--){
        *a = *b;
        a = a - 1;
        b = b - 1;
    }
}
void MainWindow::strcatch(char *a,const char *b){   //将字符数组b拼接到字符数组a的第一个'\0'后面
    while(*a != 0)
        a++;
    while(*b != 0){
        *a = *b;
        a++;
        b++;
    }
    *a =*b;
}
void MainWindow::copy(char *a,char *b){          //将字符数组b复制到字符数组a中
    while(*b != 0){
        *a = *b;
        a++;
        b++;
    }
    *a = *b;
}
int MainWindow::cmp(const char *a,const char *b,long n){     //比较字符串a和b，若相等返回0，否则返回-1
    int i = 0;
    while(i < n){
        if(*a == *b){
            a++;
            b++;
        }
        else
            return -1;
    }
    return 0;
}
int MainWindow::compress(const char *file,const char *outfile){    //读取要压缩的文件，把压缩后的文件写入另一个文档
    char b[512];
    int flag;
    float sum;
    unsigned char c;
    long i,j,m,n,f,min,p,flength;
    qDebug() << file<<outfile;
    for(int k = 0;k < 512;k ++){       //对每一个结点进行初始化
        node[k].b = 0;
        node[k].bits[0] = 0;
        node[k].count = 0;
        node[k].lch = 0;
        node[k].rch = 0;
        node[k].parent = 0;
    }
    temp.b = 0;                    //对中间结点初始化
    temp.bits[0] = 0;
    temp.count = 0;
    temp.lch = 0;
    temp.rch = 0;
    temp.parent = 0;
    FILE *ifp,*ofp;
    ifp = fopen(file,"rb");   //读取要压缩的文件
    if(ifp == NULL){          //读取失败，返回0
        qDebug() << "打开压缩文件失败";
        return 0;
    }
    ofp = fopen(outfile,"wb"); //打开压缩后的文件
    if(ifp == NULL){           //打开失败，返回0
        qDebug() << "打开压缩后文件失败";
        return 0;
    }
    flength = 0;
    while(feof(ifp) == 0){     //读文件，统计每个字符出现的次数和文件中所有字符总数
        fread(&c,1,1,ifp);
        node[c].count = node[c].count+1;
        flength++;
    }
    flength--;
    node[c].count--;
    for(i = 0;i < 512;i ++){        //将在文件中出现过的结点初始化
        if(node[i].count != 0)
            node[i].b = i;
        else
            node[i].b = -1;
        node[i].parent = -1;
        node[i].lch = -1;
        node[i].rch = -1;
    }
    for(i = 0;i < 511;i ++){      //利用冒泡排序将结点按出现次数从大到小排序；
        for(j = i + 1;j < 512;j ++){
            if(node[i].count < node[j].count){
                temp = node[i];
                node[i] = node[j];
                node[j] = temp;
            }
        }
    }
    for(i=0;i<512;i++){
        if(node[i].count == 0)
            break;
    }
    n = i;
    m = 2 * n -1;
    for(i = n;i < m;i++){         //构造具有n个叶子结点的哈夫曼树
        min = 100000;
        for(j = 0;j < i;j ++){
            flag = 1;
            if(node[j].parent != -1)
                flag = 0;
            if(node[i].count < min && flag == 1){
                p = j;
                min = node[i].count;
            }
        }
        node[i].count = node[p].count;
        node[p].parent = i;
        node[i].lch = p;
        min = 100000;
        for(j = 0;j < i;j ++){
            flag = 1;
            if(node[j].parent != -1)
                flag = 0;;
            if(node[i].count < min && flag == 1){
                p = j;
                min = node[i].count;
            }
        }
        node[i].count += node[p].count;
        node[p].parent = i;
        node[i].rch = p;
    }
    for(i = 0;i < n;i++){           //对哈夫曼树的n个叶子结点进行哈夫曼编码
        node[i].bits[0] = 0;
        f = i;
        while(node[f].parent != -1){
            j = f;
            f = node[f].parent;
            if(node[f].lch == j){
                j = strlength(node[i].bits);
                move(node[i].bits + 1,node[i].bits,j+1);
                node[i].bits[0] = '0';
            }
            else{
                j = strlength(node[i].bits);
                move(node[i].bits + 1,node[i].bits,j+1);
                node[i].bits[0] = '1';
            }
        }
    }
    fseek(ifp,0,SEEK_SET);  //将指针定在文件起始位置
    fseek(ofp,8,SEEK_SET);  //以8位二进制数为单位进行读取
    b[0] = 0;
    f = 0;
    p = 8;
    while(!feof(ifp)){      //读取要压缩的文件的每一个字符，按码表替换文件中的字符
        c = fgetc(ifp);
        f++;
        for(i = 0;i < n;i ++){
            if(c == node[i].b)
                break;
        }
        strcatch(b,node[i].bits);
        j = strlength(b);
        c = 0;
        while(j >= 8){      //当剩余字符数量不少于8个时
            for(i = 0;i < 8;i ++){    //按8位二进制数转换成十进制ASCII码写入文件，进行压缩
                if(b[i] == '1')
                    c = (c << 1) | 1;
                else
                    c = c << 1;
            }
            fwrite(&c,1,1,ofp);
            p ++;
            copy(b,b + 8);
            j = strlength(b);
        }
        if(f == flength){
            break;
        }
    }
    if(j > 0){            //当剩余字符数量小于8个时 ，先补到8个，再转换成一个字符
        strcatch(b,"00000000");
        for(i = 0;i < 8;i ++){
            if(b[i] == '1')
                c = (c << 1) |1;
            else
                c = c << 1;
        }
        fwrite(&c,1,1,ofp);
        p++;
    }
    fseek(ofp,0,SEEK_SET);
    fwrite(&flength,sizeof(flength),1,ofp);
    fwrite(&p,sizeof(long),1,ofp);
    fseek(ofp,p,SEEK_SET);
    fwrite(&n,sizeof(long),1,ofp);
    for(i = 0;i < n;i ++){
        temp = node[i];
        fwrite(&(node[i].b),1,1,ofp);
        p++;
        c = strlength(node[i].bits);
        fwrite(&c,1,1,ofp);
        p++;
        j = strlength(node[i].bits);
        if(j %8 != 0){
            for(f = j % 8;f < 8;f ++)   //将位数补为8的倍数
                strcatch(node[i].bits,"0");
        }
        while(node[i].bits[0] != 0){
            c = 0;
            for(j = 0;j < 8;j ++){
                if(node[i].bits[j] == '1')
                    c = (c << 1) | 1;
                else
                    c = c << 1;
            }
            copy(node[i].bits,node[i].bits + 8);
            fwrite(&c,1,1,ofp);
            p++;
        }
        node[i] = temp;
    }                                  //将编码信息写入文件
    fclose(ifp);                       //关闭文件
    fclose(ofp);
    sum = (float)(p + 4)/flength;
    return 1;
}

void MainWindow::on_btn_compress_clicked()
{
    if(ui->upload_line->text().isEmpty()){
        return;
    }
    else{
        QString str1 ="../coursedata/";
        str1 +=ui->upload_line->text();
        str1 +=".txt";
        QString str2 ="../coursedata/";
        str2 +=ui->upload_line->text();
        str2 +=".zip";
        QTextCodec *code=QTextCodec::codecForName("gb2312");
        std::string name1 = code->fromUnicode(str1).data();
        std::string name2 = code->fromUnicode(str2).data();
        int i =compress(name1.data(),name2.data());
        write_log("用户student压缩了"+ui->upload_line->text());
        emit in_log();
        ui->upload_line->clear();
        if(i == 0)
        {
            qDebug() << "压缩失败";
            QMessageBox::information(this,"提示","该课程资料不存在");
        }
        else if(i==1)
        {
            qDebug() << "压缩成功";
            QMessageBox::information(this,"提示","压缩成功");
        }        
    }
}

void MainWindow::on_btn_compress_2_clicked()
{
    if(ui->upload_line_2->text().isEmpty()){
        return;
    }
    else{
        QString str1 ="../homework/";
        str1 +=ui->upload_line_2->text();
        str1 +=".txt";
        QString str2 ="../homework/";
        str2 +=ui->upload_line_2->text();
        str2 +=".zip";
        QTextCodec *code=QTextCodec::codecForName("gb2312");
        std::string name1 = code->fromUnicode(str1).data();
        std::string name2 = code->fromUnicode(str2).data();
        int i =compress(name1.data(),name2.data());
        write_log("用户student压缩了"+ui->upload_line_2->text());
        emit in_log();
        ui->upload_line_2->clear();
        if(i == 0)
        {
            qDebug() << "压缩失败";
            QMessageBox::information(this,"提示","该课程资料不存在");
        }
        else if(i==1)
        {
            qDebug() << "压缩成功";
            QMessageBox::information(this,"提示","压缩成功");
        }        
    }
}
void MainWindow::timer_counter()
{
    if(i_time.second==59)
    {
        i_time.second=0;
        if(i_time.minute==59)
        {
            i_time.minute=0;
            if(i_time.hour==23)
            {
                i_time.hour=0;
                if(i_time.month==1||i_time.month==3||i_time.month==5||i_time.month==7||i_time.month==8||i_time.month==10||i_time.month==12)
                {
                    if(i_time.day==31)
                    {
                        i_time.day=1;
                        if(i_time.month==12)
                        {

                            i_time.year++;
                        }
                        else
                        {
                            i_time.month++;
                        }
                    }
                    else
                    {
                        i_time.day++;
                    }
                }
                else if(i_time.month==2)
                {
                    if(i_time.day==28)
                    {
                        i_time.day=1;
                        if(i_time.month==12)
                        {

                            i_time.year++;
                        }
                        else
                        {
                            i_time.month++;
                        }
                    }
                    else
                    {
                        i_time.day++;
                    }
                }
                else
                {
                    if(i_time.day==30)
                    {
                        i_time.day=1;
                        if(i_time.month==12)
                        {

                            i_time.year++;
                        }
                        else
                        {
                            i_time.month++;
                        }
                    }
                    else
                    {
                        i_time.day++;
                    }
                }
            }
            else
            {
                i_time.hour++;
            }
        }
        else
        {
            i_time.minute++;
        }
        clock_act();
    }
    else
    {
        i_time.second++;
    }
}
void MainWindow::read_time()
{
    QFile file("../total_information/time.txt");
    bool isOk=file.open(QIODevice::ReadOnly);
    if(isOk == true)
    {
        QTextStream in(&file);
        in >> i_time.year >> i_time.month >> i_time.day >> i_time.hour >> i_time.minute >> i_time.second;
    }
    else
    {
        qDebug() << "时间文件打开失败";
    }
    file.close();
}
void MainWindow::write_time()
{
    QFile file("../total_information/time.txt");
    bool isOk=file.open(QIODevice::WriteOnly);
    if(isOk == true)
    {
        QTextStream out(&file);
        out << i_time.year << " " << i_time.month << " " << i_time.day << " " << i_time.hour << " " << i_time.minute << " " << i_time.second;
    }
    else
    {
        qDebug() << "时间文件打开失败";
    }
    file.close();
}
void MainWindow::on_btn_stop_clicked()
{
    current_time->stop();
    write_log("用户student暂停了系统时间");
    emit in_log();
}

void MainWindow::on_btn_start_clicked()
{
    current_time->start(17);
    write_log("用户student恢复了系统时间");
    emit in_log();
}

void MainWindow::on_btn_accelerate_clicked()
{
    current_time->stop();
    current_time->start(3);
    write_log("用户student加速了系统时间");
    emit in_log();
}

void MainWindow::on_btn_upload_clicked()
{
    if(ui->search_line_2->text().isEmpty() || ui->upload_line->text().isEmpty())
    {
        QMessageBox::information(this,"提示","请输入完整的课程名和上传资料名");
    }
    else
    {
        int flag1 = 0;
        for(int i =0;i<total_course;i++)
        {
            if(item_c[i].get_course_name()==ui->search_line_2->text())
            {
                flag1 = 1;
                int j =0;
                while(item_c[i].get_coursedata()[j].flag!=-1)
                {
                    if(item_c[i].get_coursedata()[j].dataname==ui->upload_line->text())
                    {
                        item_c[i].get_coursedata()[j].flag =0;
                    }
                    j++;
                }
                item_c[i].get_coursedata()[j].dataname=ui->upload_line->text();
                item_c[i].get_coursedata()[j].flag=1;
                QTime t(i_time.hour,i_time.minute,i_time.second);
                QDate d(i_time.year,i_time.month,i_time.day);
                QDateTime cur_time(d,t);
                item_c[i].get_coursedata()[j].up_date.year=cur_time.toString("yyyy");
                item_c[i].get_coursedata()[j].up_date.month=cur_time.toString("MM");
                item_c[i].get_coursedata()[j].up_date.day=cur_time.toString("dd");
                item_c[i].get_coursedata()[j].up_date.hour=cur_time.toString("hh");
                item_c[i].get_coursedata()[j].up_date.minute=cur_time.toString("mm");
                QString str ="../total_information/" + ui->search_line_2->text() + "_coursedatalist.txt";
                str.toLocal8Bit().constData();
                QFile file1(str);
                file1.open(QIODevice::WriteOnly|QIODevice::Text|QIODevice::Append);
                QTextStream out(&file1);
                out << item_c[i].get_coursedata()[j].dataname << " "<<item_c[i].get_coursedata()[j].up_date.year
                    << " "<<item_c[i].get_coursedata()[j].up_date.month << " "<<item_c[i].get_coursedata()[j].up_date.day
                    << " "<<item_c[i].get_coursedata()[j].up_date.hour << " "<<item_c[i].get_coursedata()[j].up_date.minute<<endl;
                file1.close();
                str ="../coursedata/" + ui->search_line_2->text() + "_" + ui->upload_line->text() + ".txt";//进入课程资料文件夹储存课程资料
                str.toLocal8Bit().constData();
                QFile file2(str);
                file2.open(QIODevice::Append);
                file2.close();
                initcousedatatable();
                initcousetable();
                QMessageBox::information(this,"提示","上传资料成功");
                ui->search_line_2->clear();
                ui->upload_line->clear();
            }
        }
        if(flag1 ==0)
        {
            QMessageBox::information(this,"提示","该课程不存在");
        }
        write_log("用户student上传了课程："+ui->search_line_2->text()+"的资料："+ui->upload_line->text());
        emit in_log();
    }
}

void MainWindow::on_btn_upload_2_clicked()
{
    if(ui->search_line_3->text().isEmpty() || ui->upload_line_2->text().isEmpty())
    {
        QMessageBox::information(this,"提示","请输入完整的课程名和上传作业名");
    }
    else
    {
        int flag1 = 0;
        int flag2 = 0;
        for(int i =0;i<total_course;i++)
        {
            if(item_c[i].get_course_name()==ui->search_line_3->text())
            {
                flag1 = 1;
                int j =0;
                while(item_c[i].get_homework()[j].now!=-1)
                {
                    if(item_c[i].get_homework()[j].workname==ui->upload_line_2->text())
                    {
                        flag2=1;
                        item_c[i].get_homework()[j].now =0;
                    }
                    j++;
                }
                if(flag2==1)
                {
                    item_c[i].get_homework()[j].workname=ui->upload_line_2->text();
                    item_c[i].get_homework()[j].now = 1;
                    item_c[i].get_homework()[j].flag=1;
                    QString str ="../total_information/" + ui->search_line_3->text() + "_homeworklist.txt";
                    str.toLocal8Bit().constData();
                    QFile file1(str);
                    file1.open(QIODevice::WriteOnly|QIODevice::Text|QIODevice::Append);
                    QTextStream out(&file1);
                        out << item_c[i].get_homework()[j].workname << " "<<item_c[i].get_homework()[j].flag <<endl;
                    file1.close();
                    str ="../homework/" + ui->search_line_3->text() + "_" + ui->upload_line_2->text() + ".txt";//进入课程作业文件夹储存课程作业
                    str.toLocal8Bit().constData();
                    QFile file2(str);
                    file2.open(QIODevice::Append);
                    file2.close();
                    inithomeworktable();
                    initcousetable();
                    QMessageBox::information(this,"提示","上传作业成功");
                    ui->search_line_3->clear();
                    ui->upload_line_2->clear();
                }
            }
        }
        if(flag1 ==0)
        {
            QMessageBox::information(this,"提示","该课程不存在");
        }
        else
        {
            if(flag2==0)
            {
                QMessageBox::information(this,"提示","该课程不存在该作业");
            }
        }
        write_log("用户student上传了课程："+ui->search_line_3->text()+"的作业："+ui->upload_line_2->text());
        emit in_log();
    }
}
void MainWindow::update_act()
{
    QFile file("../total_information/activity_information.txt");
    bool isOk=file.open(QIODevice::WriteOnly);
    if(isOk == true)
    {
        QTextStream out(&file);
        for(int i=0;i<total_activity;i++)
        {
            out << item_a[i].getname() << " "<<item_a[i].gettype()<<" " <<item_a[i].get_start_time().year<<" "
                <<item_a[i].get_start_time().month<<" "<<item_a[i].get_start_time().day<<" "<<item_a[i].get_start_time().week<<" "
                <<item_a[i].get_start_time().day_of_week<<" "<<item_a[i].get_start_time().hour<<" "<<item_a[i].get_start_time().minute<<" "
                <<item_a[i].get_end_time().year<<" "
                <<item_a[i].get_end_time().month<<" "<<item_a[i].get_end_time().day<<" "<<item_a[i].get_end_time().week<<" "
                <<item_a[i].get_end_time().day_of_week<<" "<<item_a[i].get_end_time().hour<<" "<<item_a[i].get_end_time().minute<<" "
                <<item_a[i].get_activity_loc().campus <<" "<<item_a[i].get_activity_loc().position<<" "<< item_a[i].get_activity_loc().n<<endl;
        }
    }
    else
    {
        qDebug() << "总课外活动信息文件打开失败";
    }
    file.close();
}

void MainWindow::on_btnadd_act_clicked()
{
    input_a.show();
}

void MainWindow::on_btndelete_act_clicked()
{
    bool ok;
    int choose;
    QString text=QInputDialog::getText(this,tr("选择活动"),tr("请输入要删除活动的编号"),QLineEdit::Normal,0,&ok);
    if(ok && !text.isEmpty())
    {
        qDebug() << "开始删除活动";
        choose=text.toInt()-1;
        write_log("用户student删除了"+item_a[a[choose]].gettype()+item_a[a[choose]].getname());
        emit in_log();
        for(int j=a[choose];j<total_activity-1;j++)
        {
            item_a[j]=item_a[j+1];
        }
        total_activity--;
        qDebug() << "删除完成";
        update_act();
        QMessageBox::information(this,tr("提示"),tr("删除完成！"));
        ui->activity_table->clear();
        initactivitytable();
        ui->btn_sort_act->setEnabled(true);
        ui->btncancel_act->setEnabled(false);
        ui->btn_sort_act->setEnabled(true);
        ui->btndelete_act->setEnabled(false);
        ui->btnupdate_act->setEnabled(false);
        ui->act_search_line->clear();
        num = 0;        
    }
}

void MainWindow::on_btnupdate_act_clicked()
{
    bool ok;
    QString text=QInputDialog::getText(this,tr("选择活动"),tr("请输入要修改活动的编号"),QLineEdit::Normal,0,&ok);
    if(ok && !text.isEmpty())
    {
        choose=text.toInt()-1;
        input_b.show();
    }
}
void MainWindow::update_log()
{
    adminwindow.get_ui()->log_list->clear();
    QFile file("../total_information/log.txt");
    bool isOk=file.open(QIODevice::ReadOnly);
    QTextStream in(&file);
    if(isOk==true)
    {
        while(!in.atEnd())
        {
            QString str,date,time,day_of_week;
            in >> date >> day_of_week >> time >> str;
            str=date + " " + day_of_week + " " + time + " " + str;
            adminwindow.get_ui()->log_list->insertItem(0,str);
        }
    }
    else
    {
        qDebug() << "日志文件打开失败";
    }
    file.close();
}
void MainWindow::write_log(QString str)
{
    QFile file("../total_information/log.txt");
    bool isOk=file.open(QIODevice::Append);
    QTextStream out(&file);
    if(isOk==true)
    {
        QTime t(i_time.hour,i_time.minute,i_time.second);
        QDate d(i_time.year,i_time.month,i_time.day);
        QDateTime cur_time(d,t);
        QString str_time=cur_time.toString("yyyy-MM-dd dddd hh:mm:ss ");
        str = str_time + str;
        out << str << "\n";
    }
    else
    {
        qDebug() << "日志文件打开失败";
    }
    file.close();
}

void MainWindow::on_btn_sort_act_clicked()
{
    quicksort(item_a,0,total_activity-1);
    ui->activity_table->clear();
    initactivitytable();
}

void MainWindow::initactivity1table()
{
    QStringList headers;
    headers << QObject::tr("序号")
            << QObject::tr("活动名称")
            << QObject::tr("活动时间")
            << QObject::tr("活动地点");
    this->ui->activity1_table->setColumnCount(headers.size()); //设置列数
    this->ui->activity1_table->setHorizontalHeaderLabels(headers);//列名称
    this->ui->activity1_table->setRowCount(total_activity);//设置行数
    this->ui->activity1_table->setAlternatingRowColors(true);//行间隔色
    this->ui->activity1_table->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);//自动列宽
    this->ui->activity1_table->installEventFilter(this);
    for(int row = 0;row < total_activity;row++)
    {
        int colindex = 0;
        this->ui->activity1_table->setItem(row,colindex++,new QTableWidgetItem(QString::number(row,10)));
        this->ui->activity1_table->setItem(row,colindex++,new QTableWidgetItem(item_a[row].getname()));
        QString str = QString("%6-%7-%8 %1 %2:%3-%4:%5").arg(item_a[row].get_start_time().day_of_week)
                                                        .arg(item_a[row].get_start_time().hour)
                                                        .arg(item_a[row].get_start_time().minute)
                                                        .arg(item_a[row].get_end_time().hour)
                                                        .arg(item_a[row].get_end_time().minute)
                                                        .arg(item_a[row].get_start_time().year)
                                                        .arg(item_a[row].get_start_time().month)
                                                        .arg(item_a[row].get_start_time().day);
        this->ui->activity1_table->setItem(row,colindex++,new QTableWidgetItem(str));
        if(item_a[row].get_activity_loc().campus == "无")
        {
            this->ui->activity1_table->setItem(row,colindex++,new QTableWidgetItem(item_a[row].get_activity_loc().position));
        }
        else
        {
            str = QString("%1 %2").arg(item_a[row].get_activity_loc().campus).arg(item_a[row].get_activity_loc().position);
            this->ui->activity1_table->setItem(row,colindex++,new QTableWidgetItem(str));
        }
    }
}
void MainWindow::initclocktable()
{
    QFile file("../total_information/clock_information.txt");
    bool isOk = file.open(QIODevice::ReadOnly);
    if(isOk == true)
    {
        QTextStream in(&file);
        total_clock=0;
        for(int j = 0 ; !in.atEnd() ; j++)
        {
            QString str;
            in >> clock[j].type;
            if(clock[j].type == 1)
            {
                in >> clock[j].year >> clock[j].month >> clock[j].day >> clock[j].hour >> clock[j].minute >> clock[j].str >> str;
                clock[j].str =clock[j].str+ " " + str;
            }
            else if(clock[j].type == 2)
            {
                in >> clock[j].day_of_week >> clock[j].hour >> clock[j].minute >> clock[j].str >> str;
                clock[j].str =clock[j].str+ " " + str;
            }
            else
            {
                in >> clock[j].hour >> clock[j].minute >> clock[j].str;
            }
            total_clock++;
        }
        total_clock--;
    }
    else
    {
        qDebug()<<"闹钟信息文件打开失败";
    }
    file.close();
    QStringList headers;
    headers << QObject::tr("序号")
            << QObject::tr("类型")
            << QObject::tr("时间");
    this->ui->clock_table->setColumnCount(headers.size()); //设置列数
    this->ui->clock_table->setHorizontalHeaderLabels(headers);//列名称
    this->ui->clock_table->setRowCount(total_clock);//设置行数
    this->ui->clock_table->setAlternatingRowColors(true);//行间隔色
    this->ui->clock_table->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);//自动列宽
    this->ui->clock_table->installEventFilter(this);
    for(int row = 0;row < total_clock;row++)
    {
        int colindex = 0;
        this->ui->clock_table->setItem(row,colindex++,new QTableWidgetItem(QString::number(row,10)));
        this->ui->clock_table->setItem(row,colindex++,new QTableWidgetItem(QString::number(clock[row].type,10)));
        this->ui->clock_table->setItem(row,colindex++,new QTableWidgetItem(clock[row].str));
    }
}

void MainWindow::write_clock()
{
    QFile file("../total_information/clock_information.txt");
    bool isOk = file.open(QIODevice::WriteOnly);
    if(isOk == true)
    {
        QTextStream out(&file);
        for(int j = 0 ; j < total_clock ; j++)
        {
            out << clock[j].type << " ";
            if(clock[j].type == 1)
            {
                out << clock[j].year <<" "<< clock[j].month <<" " << clock[j].day <<" "<<  clock[j].hour << " " <<  clock[j].minute << " "
                    << clock[j].str << "\n";
            }
            else if(clock[j].type == 2)
            {
                out <<  clock[j].day_of_week << " "<< clock[j].hour <<" " << clock[j].minute<<" "
                    << clock[j].str << "\n";
            }
            else if(clock[j].type == 3)
            {
                out << clock[j].hour << " " << clock[j].minute << " "
                    << clock[j].str << "\n";
            }
        }
    }
    else
    {
        qDebug()<<"闹钟信息文件打开失败";
    }
    file.close();
}

void MainWindow::add_clock1()
{
    if(ui->line_add_clock->text().isEmpty())
    {
        return;
    }
    else
    {
        int i;
        i = ui->line_add_clock->text().toInt();
        if(i < total_activity)
        {
            vtime t;
            t = item_a[i].get_start_time();
            clock[total_clock].type = 1;
            clock[total_clock].year = t.year.toInt();
            clock[total_clock].month = t.month.toInt();
            clock[total_clock].day = t.day.toInt();
            clock[total_clock].hour = t.hour.toInt();
            clock[total_clock].minute = t.minute.toInt();
            QString cstr;
            cstr = t.year+"-"+t.month+"-"+t.day+" "+t.hour+":"+t.minute;
            write_log("用户student添加了时间为"+cstr+"的单个闹钟");
            emit in_log();
            clock[total_clock].str = cstr;
            ++total_clock;
            write_clock();
            initclocktable();
        }
        else
        {
            QMessageBox::information(this,"提示","该活动不存在");
        }
        ui->line_add_clock->clear();
    }
}
int MainWindow:: day_of_week_transfer(QString g)
{
    if(g.compare("星期一") == 0)return 1;
    else if(g.compare("星期二") == 0)return 2;
    else if(g.compare("星期三") == 0)return 3;
    else if(g.compare("星期四") == 0)return 4;
    else if(g.compare("星期五") == 0)return 5;
    else if(g.compare("星期六") == 0)return 6;
    else return 7;
}
void MainWindow::add_clock2()
{
    if(ui->line_add_clock->text().isEmpty())
    {
        return;
    }
    else
    {
        int i;
        i = ui->line_add_clock->text().toInt();
        if(i < total_activity)
        {
            vtime t;
            t = item_a[i].get_start_time();
            clock[total_clock].type = 2;
            clock[total_clock].day_of_week = day_of_week_transfer(t.day_of_week);
            clock[total_clock].hour = t.hour.toInt();
            clock[total_clock].minute = t.minute.toInt();
            QString cstr;
            cstr = t.day_of_week+" "+t.hour+":"+t.minute;
            write_log("用户student添加了时间为"+cstr+"的每周闹钟");
            emit in_log();
            clock[total_clock].str = cstr;
            ++total_clock;
            write_clock();
            initclocktable();
        }
        else
        {
            QMessageBox::information(this,"提示","该活动不存在");
        }
        ui->line_add_clock->clear();
    }
}
void MainWindow::add_clock3()
{
    if(ui->line_add_clock->text().isEmpty())
    {
        return;
    }
    else
    {
        int i;
        i = ui->line_add_clock->text().toInt();
        if(i < total_activity)
        {
            vtime t;
            t = item_a[i].get_start_time();
            clock[total_clock].type = 3;
            clock[total_clock].hour = t.hour.toInt();
            clock[total_clock].minute = t.minute.toInt();
            QString cstr;
            cstr = t.hour+":"+t.minute;
            write_log("用户student添加了时间为"+cstr+"的每日闹钟");
            emit in_log();
            clock[total_clock].str = cstr;
            ++total_clock;
            write_clock();
            initclocktable();
        }
        else
        {
             QMessageBox::information(this,"提示","该活动不存在");
        }
         ui->line_add_clock->clear();
    }
}
void MainWindow::on_btn_delete_clock_clicked()
{
    if(ui->line_delete_clock->text().isEmpty())
    {
        return;
    }
    else
    {
        int i;
        i = ui->line_delete_clock->text().toInt();
        if(i < total_clock)
        {
            write_log("用户student删除了时间为"+clock[i].str+"的闹钟");
            emit in_log();
            for( int n = i ; n < total_clock ; n++ )
            {
                clock[n] = clock[n+1];
            }
            total_clock--;
            write_clock();
            initclocktable();
        }
        else
        {
             QMessageBox::information(this,"提示","该闹钟不存在");
        }
         ui->line_delete_clock->clear();
    }
}

void MainWindow::on_btn_add_clock_clicked()
{
    clock_add_menu->exec(QPoint(QCursor::pos().x(),QCursor::pos().y()));
}
void MainWindow::clock_act()
{
    int i;
    QDate d(i_time.year,i_time.month,i_time.day);
    for(i = 0 ; i < total_clock ; i++)
    {
        switch (clock[i].type)
        {
        case 1:
            if(i_time.year == clock[i].year && i_time.month == clock[i].month && i_time.day == clock[i].day && i_time.hour == clock[i].hour && i_time.minute == clock[i].minute && adminwindow.isHidden())
            {
                ring->play();
                QMessageBox::StandardButton r = QMessageBox::information(this,"提示","闹钟响了");
                if(r == QMessageBox::Ok)ring->stop();
            }
            break;
        case 2:
        {
             int dow = d.dayOfWeek();
             if(dow == clock[i].day_of_week && i_time.hour == clock[i].hour && i_time.minute == clock[i].minute && adminwindow.isHidden())
             {
                 ring->play();
                 QMessageBox::StandardButton r = QMessageBox::information(this,"提示","闹钟响了");
                 if(r == QMessageBox::Ok)ring->stop();
             }
        }
            break;
        case 3:
            if(i_time.hour == clock[i].hour && i_time.minute == clock[i].minute && adminwindow.isHidden())
            {
                ring->play();
                QMessageBox::StandardButton r = QMessageBox::information(this,"提示","闹钟响了");
                if(r == QMessageBox::Ok)ring->stop();
            }
            break;
        default:
            break;
        }
    }
}
