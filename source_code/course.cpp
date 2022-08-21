#include "course.h"
#include<QFile>
#include<QIODevice>
#include <QDebug>

course::course()
{
    data[0].up_date.year=-1;
    course_work[0].flag=-1;
}
QTextStream &read_homework(QTextStream &is,homework &item)
{
    item.now = 1;//将作业的新旧标识为置为新
    is >> item.workname >> item.flag;
    return is;
}//将文件中的作业名称和标识符信息存入作业类的数组中
QTextStream &read_coursedata(QTextStream &is,course_data &item)
{
    item.flag = 1;//将课程资料的新旧标识为置为新
    is >> item.dataname >> item.up_date.year >> item.up_date.month >> item.up_date.day >> item.up_date.hour >> item.up_date.minute;
    return is;
}//将文件中的课程资料名称和上传时间存入资料类的数组中
QTextStream &read_course(QTextStream &is, course &item )
{
    item.start_time.week = "-1";
    item.end_time.week = "-1";//将课程的开始时间和结束时间所在周都置为-1，表示每周都有课
    is >> item.course_name //输入课程名称
       >> item.class_loc.n >> item.class_loc.position >> item.class_loc.campus//输入课程地点的坐标和名称
       >> item.start_time.day_of_week >> item.start_time.hour >> item.start_time.minute
       >> item.end_time.hour >> item.end_time.minute//输入课程的时间信息
       >> item.teacher_name //输入教师名称
       >> item.cur_section >> item.total_section//输入课程节数
       >> item.course_data_filename >> item.homework_filename//输入课程资料和作业的清单名称
       >> item.course_group //输入课程群
       >> item.exam_start_time.year >> item.exam_start_time.month >> item.exam_start_time.day
       >> item.exam_start_time.week >> item.exam_start_time.day_of_week >> item.exam_start_time.hour >> item.exam_start_time.minute
       >> item.exam_end_time.hour >> item.exam_end_time.minute//输入考试时间
       >> item.exam_loc.n >> item.exam_loc.position >> item.exam_loc.campus;//输入考试地点的坐标和名称
    item.homework_filename = "../total_information/" + item.homework_filename + ".txt";

    QFile filehomework(item.homework_filename);
    bool isOk = filehomework.open(QIODevice::ReadOnly);
    if(isOk == true)
    {
        QTextStream in(&filehomework);
        int i = 0;
        while(!read_homework(in,item.course_work[i]).atEnd())
        {
            int j = 0;
            while(j < i)
            {
                if(!item.course_work[j].workname.compare(item.course_work[i].workname))
                {
                    item.course_work[j].now = 0;
                }//若资料中出现同名旧作业则将其标识位，置为0
                j++;
            }
            i++;
        }
        item.course_work[i].now = -1;
        item.course_work[i].flag = -1;//将作业清单中的信息存入作业的对象数组中
        filehomework.close();
    }
    item.course_data_filename = "../total_information/" + item.course_data_filename + ".txt";
    QFile filedata(item.course_data_filename);
    isOk = filedata.open(QIODevice::ReadOnly);
    if(isOk == true)
    {
        QTextStream in(&filedata);
        int i = 0;
        while(!read_coursedata(in,item.data[i]).atEnd())
        {
            int j = 0;
            while(j < i)
            {
                if(!item.data[j].dataname.compare(item.data[i].dataname))
                {
                    item.data[j].flag = 0;
                }//若资料中出现同名旧资料则将其标识位置为0
                j++;
            }
            i++;
        }
        item.data[i].flag = -1;//将课程资料清单中的信息存入课程资料的对象数组中
        filedata.close();
    }
    return is;
}//输入课程的各项信息
int day_of_week_change(QString a)
{
    if(a=="星期一")
    {
        return 1;
    }
    else if(a=="星期二")
    {
        return 2;
    }
    else if(a=="星期三")
    {
        return 3;
    }
    else if(a=="星期四")
    {
        return 4;
    }
    else if(a=="星期五")
    {
        return 5;
    }
    else if(a=="星期六")
    {
        return 6;
    }
    else
    {
        return 7;
    }
}
int time_compare(vtime a,vtime b)
{
    if(day_of_week_change(a.day_of_week)<day_of_week_change(b.day_of_week))
    {
        return -1;
    }
    else if(day_of_week_change(a.day_of_week)>day_of_week_change(b.day_of_week))
    {
        return 1;
    }
    else
    {
        if(a.hour.toInt()<b.hour.toInt())
        {
            return -1;
        }
        else if(a.hour.toInt()>b.hour.toInt())
        {
            return 1;
        }
        else
        {
            if(a.minute.toInt()>b.minute.toInt())
            {
                return 1;
            }
            else if(a.minute.toInt()<b.minute.toInt())
            {
                return -1;
            }
            else
            {
                return 0;
            }
        }
    }
}
int course_compare(course a,course b,int k)
{
    int t = 0;
    switch(k)
    {
        case 1:
        t = QString::compare(a.course_name,b.course_name,Qt::CaseInsensitive);
        break;//k为1时根据课程名称排序
        case 2:
        {
            t = QString::compare(a.class_loc.campus,b.class_loc.campus,Qt::CaseInsensitive);//k为2时根据校区进行排序
            if( t == 0 )
            {
            t = QString::compare(a.class_loc.position,b.class_loc.position,Qt::CaseInsensitive);//校区相同时根据课程地点进行排序
            }
        }
        break;
        case 3:
        {
            t=time_compare(a.get_start_time(),b.get_start_time());
        }
        break;

    }
    return t;
}//课程比较
void quicksort(course *item,int begin,int end,int k)
{
    if(begin > end)
    {
        return;
    }
    int i,j;
    course temp;
    temp = item[begin];
    i = begin;
    j = end ;
    while(i < j)
    {
        while( i < j && course_compare(item[j] , temp , k) > 0)
        {
            j--;
        }
        item[i] = item[j];
        while( i < j && course_compare(item[i] , temp , k) <= 0 )
        {
            i++;
        }
        item[j] = item[i];
    }
    item[i] = temp;
    quicksort(item, begin, i-1,k);
    quicksort(item, i+1,end, k);
}//快速排序



void input_c(course &item,QString course_name,location class_loc,vtime start_time,vtime end_time,QString teacher_name,int total_section,QString course_group,vtime exam_start_time,vtime exam_end_time,location exam_loc)
{
    item.course_name=course_name;
    item.class_loc=class_loc;
    item.start_time=start_time;
    item.end_time=end_time;
    item.teacher_name=teacher_name;
    item.total_section=total_section;
    item.cur_section=1;
    item.progress_percent=1/item.total_section;
    item.course_group=course_group;
    item.homework_filename=course_name+"_homeworklist.txt";
    item.course_data_filename=course_name+"_coursedatalist.txt";
    item.exam_start_time=exam_start_time;
    item.exam_end_time=exam_end_time;
    item.exam_loc=exam_loc;
    item.data[0].up_date.year=-1;
    item.course_work[0].flag=-1;
    item.homework_filename = "../total_information/" + item.homework_filename + ".txt";
    QFile filehomework(item.homework_filename);
    bool isOk = filehomework.open(QIODevice::ReadOnly);
    if(isOk == true)
    {
        QTextStream in(&filehomework);
        int i = 0;
        while(!read_homework(in,item.course_work[i]).atEnd())
        {
            int j = 0;
            while(j < i)
            {
                if(!item.course_work[j].workname.compare(item.course_work[i].workname))
                {
                    item.course_work[j].now = 0;
                }//若资料中出现同名旧作业则将其标识位，置为0
                j++;
            }
            i++;
        }
        item.course_work[i].flag = -1;//将作业清单中的信息存入作业的对象数组中
        filehomework.close();
    }
    item.course_data_filename = "../total_information/" + item.course_data_filename + ".txt";
    QFile filedata(item.course_data_filename);
    isOk = filedata.open(QIODevice::ReadOnly);
    if(isOk == true){
        QTextStream in(&filedata);
        int i = 0;
        while(!read_coursedata(in,item.data[i]).atEnd())
        {
            int j = 0;
            while(j < i)
            {
                if(!item.data[j].dataname.compare(item.data[i].dataname))
                {
                    item.data[j].flag = 0;
                }//若资料中出现同名旧资料则将其标识位置为0
                j++;
            }
            i++;
        }
        item.data[i].up_date.year = -1;//将课程资料清单中的信息存入课程资料的对象数组中
        filedata.close();
    }
}
