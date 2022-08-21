#ifndef ACTIVITY_H
#define ACTIVITY_H
#include"course.h"
#include<QTextStream>
#include<QString>
#include <QObject>

class activity
{
    friend QTextStream& read_activity(QTextStream&, activity&);//从文件中读入活动的信息
    friend void quicksort(activity*,int,int);
public:
    activity();
    int activity_time_compare(vtime a,vtime b);
    QString getname(){return activity_name;}//返回活动名称
    QString gettype(){ return activity_type;}//返回活动类型
    location get_activity_loc(){ return activity_location;}
    vtime get_start_time(){ return start_time;}//返回活动开始时间
    vtime get_end_time(){ return end_time;}//返回活动结束时间
    activity operator=(const activity&);//赋值运算符重载
    void get_value(QString name,QString type,location location,vtime start,vtime end);//给活动对象赋值函数
private:
    QString activity_name;//活动名称
    location activity_location;//活动地点
    vtime start_time;//活动开始时间
    vtime end_time;//活动结束时间
    QString activity_type;//活动类型
};

#endif // ACTIVITY_H
