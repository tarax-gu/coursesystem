#ifndef COURSE_H
#define COURSE_H
#include<QString>
#include <QObject>
#include<QTextStream>

struct vtime
{

    QString week,day_of_week,hour,minute, year,month,day;
};
struct  clock_time
{
    int type;
    int year;
    int month;
    int day;
    int hour;
    int minute;
    int day_of_week;
    QString str;
};//闹钟的时间
struct location
{

    int n;
    QString position;
    QString campus;
};
struct building //建筑
{
    int area;   //校区
    int n;      //编号
    QString name; //地名
};
struct path //路
{
    int area;      //校区
    int a;         //路一端
    int b;         //路另一端
    int length;    //长度
    float crowd;   //拥挤指数
    int kind;      //种类，2为人行道，1为普通道路
};
struct course_data
{

    int flag = -1;//一个标识位，若为0则为旧版
    vtime up_date;
    QString dataname;
};
struct homework
{
    int flag = -1;
    int now = -1; //一个标识符，若为0则为旧版
    QString workname;
};
class course
{
    friend QTextStream &read_course(QTextStream &, course &);//从文件中读入课程信息
    friend QTextStream &read_homework(QTextStream &, homework &);//从文件中读取课程作业信息
    friend QTextStream &read_coursedata(QTextStream &, course_data &);//从文件中读取课程资料信息
	friend int course_compare(course ,course ,int );//比较两个课程对象 
    friend void quicksort(course*,int,int,int);
	friend void search(); //查询课程
    friend void input_c(course &item,QString course_name,location class_loc,vtime start_time,vtime end_time,QString teacher_name,int total_section,QString course_group,vtime exam_start_time,vtime exam_end_time,location exam_loc);
public:
    course();
    QString get_course_name(){ return course_name;}//返回课程名称
    location get_course_location(){ return class_loc;}//返回课程地点
    vtime get_start_time(){ return start_time;}//返回课程开始时间
    vtime get_end_time(){ return end_time;}//返回课程结束时间
    QString get_teacher_name(){ return teacher_name;}//返回教师名字
    int get_total_section(){ return total_section;};//返回课程总节数
    int get_cur_section(){ return cur_section;};//返回当前课程节数
    QString get_course_group(){ return course_group;}//返回课程群
    homework* get_homework(){ return course_work;}//返回储存作业信息的数组
    course_data* get_coursedata(){ return data;}//返回储存课程资料信息的数组
    vtime get_exam_start_time(){ return exam_start_time;}//返回考试开始时间
    vtime get_exam_end_time(){ return exam_end_time;}//返回考试结束时间
    location get_exam_loc(){ return exam_loc;}//返回考试地点
    QString get_data_filename(){return course_name+"_coursedatalist";}//返回课程资料文件清单名称
    QString get_work_filename(){return course_name+"_homeworklist";}//返回作业文件清单名称
    int time_compare();//时间比较函数
    int day_of_week_change(QString);
private:
    QString course_name;//课程名称
    location class_loc;//课程地点
    vtime start_time;//课程开始时间
    vtime end_time;//课程结束时间
    QString teacher_name;//教师名字
    int total_section;//课程总节数
    int cur_section;//当前课程节数
    double progress_percent;//课程进度百分比
    QString course_group;//课程群
    QString homework_filename; //作业清单文件名称
    QString course_data_filename;//课程资料清单文件名称
    homework course_work[40];//储存作业信息的数组
    course_data data[40];//储存课程资料信息的数组
    vtime exam_start_time;//考试开始时间
    vtime exam_end_time;//考试结束时间
    location exam_loc;//考试地点
};

#endif // COURSE_H
