#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include"page_login.h"
#include"course.h"
#include"activity.h"
#include<QString>
#include<QAction>
#include<QMenu>
#include<QTimer>
#include <QMainWindow>
#include    <QMediaPlayer>
#include "images_change.h"
#include "administorwindow.h"
#include"input_activity.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE
struct imitate_time
{
    int year=0,month=0,day=0,hour=0,minute=0,second=0;
};
struct Node{//定义结点
    int b;                   //字符
    long count;              //文件中该字符出现的次数
    long parent,lch,rch;     //该结点在哈夫曼数中的父母结点、左孩子结点和右孩子结点
    char bits[256];          //该结点对应的哈夫曼编码
};
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    Node node[512];
    Node temp;
    images_change ic;//创建一个图片切换类作为MainWindow的成员变量
    course item_c[20];
    int total_course = 0;//记录总课程数
    activity item_a[40];
    int num =0,choose=0;
    int a[40];
    int total_activity = 0;//记录总活动数
    int total_homework = 0;
    int total_data = 0;
    int total_path = 0;    //记录总路数
    int total_building = 0; //记录总建筑数
    int pre_p[200] = { 0 };  //记录路径
    path p[300];            //路
    building b[300];        //建筑
    int cost[201][201];     //邻接表
    int bushour[5];         //校车发车小时
    int busmin[5];          //校车发车分
    clock_time clock[15];     //闹钟
    int total_clock = 0;         //闹钟总数
    QMediaPlayer *ring = new QMediaPlayer;
    void read_all_information();
    void read_path();
    void dijkstra(int vis[], int dis[], int start, int goal);  //迪杰斯特拉算法
    QString output_path(int start, int goal);  //输出路径
    void search_shortest_path(int start, int goal);  //找寻并输出路径
    void on_treewidgetitem_changed();
    void initcousetable();
    void initactivitytable();
    void initcousedatatable();
    void inithomeworktable();
    void initactivity1table();
    void initclocktable();
    void sort1();//将课程信息按课程名排序槽函数
    void sort2();//将课程信息按上课地点所在校区排序槽函数
    void sort3();//将课程信息按上课时间排序槽函数
    void guide1();//按课程名导航槽函数
    void guide2();//按上课时间导航槽函数
    void guide3();//按地点导航槽函数
    int compare_time(vtime ,vtime );//比较时间
    int activity_collision_detect(vtime,vtime,vtime,vtime);//对活动与活动或活动与课程进行时间冲突检查,若出现冲突则返回1，否则返回0
    void detect_collision(course *item_c,activity *item_a);
    int strlength(char *a);
    void move(char *a,char *b, long n);
    void strcatch(char *a,const char *b);
    void copy(char *a,char *b);
    int cmp(const char *a,const char *b,long n);
    int compress(const char *file,const char *outfile);
    void timer_counter();
    void read_time();
    void write_time();
    void update_act();
    void write_log(QString);
    void add_clock1();
    int day_of_week_transfer(QString g);
    void add_clock2();
    void add_clock3();
    void clock_act();
    void write_clock();

signals:
    void in_log();
private slots:

    void on_btn_course_sort_clicked();
    void on_btnsearch_act_clicked();
    void on_btncancel_act_clicked();
    void on_map_btn_clicked();
    void on_btn_search_clicked();
    void on_btn_cancel_search_clicked();
    void on_btn_search_2_clicked();
    void on_btn_cancel_search_2_clicked();
    void on_btn_search_3_clicked();
    void on_btn_cancel_search_3_clicked();
    void on_btn_compress_clicked();
    void on_btn_compress_2_clicked();
    void on_btn_stop_clicked();
    void on_btn_start_clicked();
    void on_btn_accelerate_clicked();
    void on_btn_upload_clicked();
    void on_btn_upload_2_clicked();
    void clear_homework_table();
    void clear_data_table();
    void on_btnadd_act_clicked();
    void on_btndelete_act_clicked();
    void on_btnupdate_act_clicked();
    void update_log();
    void on_btn_sort_act_clicked();
    void on_btn_delete_clock_clicked();
    void on_btn_add_clock_clicked();

private:
    Ui::MainWindow *ui;
    page_login loginpage;
    administerWindow adminwindow;
    imitate_time i_time;
    input_activity input_a;
    input_activity input_b;
    QTimer *current_time=new QTimer(this);
    QMenu *course_sort_menu = NULL;
    QAction *act1 = NULL;
    QAction *act2 = NULL;
    QAction *act3 = NULL;
    QMenu *course_guide_menu = NULL;
    QAction *act4 = NULL;
    QAction *act5 = NULL;
    QAction *act6 = NULL;
    QMenu *clock_add_menu = NULL;
    QAction *act7 = NULL;
    QAction *act8 = NULL;
    QAction *act9 = NULL;
};
#endif // MAINWINDOW_H
