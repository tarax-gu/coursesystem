#include <iostream>
#include <string>
#include <fstream>
using namespace std;
struct time
{
    string weekday,hour, minute;
};//定义一个类，包含星期、时、分
struct location
{
    int x,y;
    string position;
    string campus;
};//定义一个类，包含地点的名称、坐标和校区
struct homework
{
    int flag;
    string workname;
};//定义一个类，储存作业的信息,flag为1表示已交，为0表示未交 
struct course_data
{
    int up_day, up_month, up_year;
    string dataname;
};//定义一个类，储存课程资料的信息
class course
{
    friend istream &read_course(istream &, course &);//从文件中读入课程信息
    friend ostream &print_course(ostream &, course &);//打印课程信息
    friend istream &read_homework(istream &, homework &);//从文件中读取课程作业信息
    friend istream &read_coursedata(istream &, course_data &);//从文件中读取课程资料信息
	friend int course_compare(course ,course ,int );//比较两个课程对象 
	friend void search(); //查询课程 
public:
    
private:
    string course_name;//课程名称
    location class_loc;//课程地点
    time start_time;//课程开始时间
    time end_time;//课程结束时间
    string teacher_name;//教师名字
    int total_section;//课程总节数
    int cur_section;//当前课程节数
    double progress_percent;//课程进度百分比
    string course_group;//课程群
    string homework_filename; //作业清单文件名称
    string course_data_filename;//课程资料清单文件名称
    homework course_work[100];//储存作业信息的数组
    course_data data[100];//储存课程信息的数组
    time exam_start_time;//考试开始时间
    time exam_end_time;//考试结束时间
    int exam_week;//考试所在周
    location exam_loc;//考试地点
};//定义一个课程类，包含课程的信息
istream &read_homework(istream &is,homework &item)
{
    is >> item.workname >> item.flag;
    return is;
}//将文件中的作业名称和标识符信息存入作业类的数组中
istream &read_coursedata(istream &is,course_data &item)
{
    is >> item.dataname >> item.up_year >> item.up_month >> item.up_day;
    return is;
}//将文件中的课程资料名称和上传时间存入资料类的数组中
istream &read_course(istream &is, course &item )
{
    is >> item.course_name //输入课程名称
       >> item.class_loc.x >> item.class_loc.y >> item.class_loc.position >> item.class_loc.campus//输入课程地点的坐标和名称
       >> item.start_time.weekday >> item.start_time.hour >> item.start_time.minute
       >> item.end_time.hour >> item.end_time.minute//输入课程的时间信息
       >> item.teacher_name //输入教师名称
       >> item.cur_section >> item.total_section//输入课程节数
       >> item.course_data_filename >> item.homework_filename//输入课程资料和作业的清单名称
       >> item.course_group //输入课程群
       >> item.exam_week >> item.exam_start_time.weekday >> item.exam_start_time.hour >> item.exam_start_time.minute
       >> item.exam_end_time.hour >> item.exam_end_time.minute//输入考试时间
       >> item.exam_loc.x >> item.exam_loc.y >> item.exam_loc.position;//输入考试地点的坐标和名称
    item.homework_filename += ".txt"; 
	ifstream in;
    in.open(item.homework_filename.c_str());
    int i = 0;
    while(read_homework(in,item.course_work[i]))
    {
        i++;
    }
    in.close();
    item.course_work[i].flag = -1;//将作业清单中的信息存入作业的对象数组中
    item.course_data_filename += ".txt";
    in.open(item.course_data_filename.c_str());
    i = 0;
    while(read_coursedata(in,item.data[i]))
    {
        i++;
    }
    in.close();
    item.data[i].up_year = -1;//将课程资料清单中的信息存入课程资料的对象数组中
    return is;
}//输入课程的各项信息
ostream &print_course(ostream &os,course &item)
{
    os << "课程名称：" << item.course_name << "\t上课地点：" << item.class_loc.position << "\t上课校区：" << item.class_loc.campus
       << "\t上课时间：" << item.start_time.weekday << " " << item.start_time.hour<< ":" << item.start_time.minute<< "-"
       << item.end_time.hour <<":"<< item.end_time.minute
       << "\t教师姓名：" << item.teacher_name << "\t当前节数：" << item.cur_section << "\t课程总节数：" << item.total_section
       << "\t课程群：" << item.course_group 
       << "\t考试时间：" <<item.exam_week << "周\t" << item.exam_start_time.weekday << " " << item.exam_start_time.hour << ":" << item.exam_start_time.minute
       << "-" << item.exam_end_time.hour << ":" << item.exam_end_time.minute
       << "\t考试地点：" << item.exam_loc.position; 
    int i = 0;
    while(item.course_work[i].flag!=-1)
    {
        os << "作业名称：" << item.course_work[i].workname << " "
           << "提交状态：";
        if(item.course_work[i].flag==1)
        {
            os << "已交\t";
        }
        else
        {
            os << "未交\t";
        }
        i++;
    }//将作业的提交情况输出
    i = 0;
    while(item.data[i].up_year!=-1)
    {
        os << "课程资料：" << item.data[i].dataname << " 上传时间："
           << item.data[i].up_year << "/" << item.data[i].up_month << "/" << item.data[i].up_day << '\t';
        i++;
    }//将课程资料的情况输出
    os << endl; 
    return os;
}//输出课程的各项信息
