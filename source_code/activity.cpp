#include "activity.h"

activity::activity()
{

}
QTextStream& read_activity(QTextStream &is, activity &item )
{
    is >> item.activity_name >> item.activity_type//输入活动的名称和类型
       >> item.start_time.year >> item.start_time.month >> item.start_time.day
       >> item.start_time.week >> item.start_time.day_of_week >> item.start_time.hour >> item.start_time.minute//输入活动的开始时间
       >> item.end_time.year >> item.end_time.month >> item.end_time.day
       >> item.end_time.week >> item.end_time.day_of_week >> item.end_time.hour >> item.end_time.minute//输入活动的结束时间
       >> item.activity_location.campus >> item.activity_location.position >> item.activity_location.n;//输入活动的地点坐标
    return is;
}//将文件中的信息读入活动类对象中
activity activity::operator =(const activity &b)
{
    activity_name = b.activity_name;
    activity_type = b.activity_type;
    start_time = b.start_time;
    end_time = b.end_time;
    activity_location = b.activity_location;
    return *this;
}//活动类的赋值运算符重载
void activity::get_value(QString name, QString type, location location, vtime start, vtime end)
{
    activity_name =name;
    activity_type =type;
    activity_location =location;
    start_time =start;
    end_time =end;
}
int activity_time_compare(vtime a,vtime b)
{
    if(a.year.toInt() < b.year.toInt())
    {
        return -1;
    }
    else if(a.year.toInt() > b.year.toInt())
    {
        return 1;
    }
    else
    {
        if(a.month.toInt() < b.month.toInt())
        {
            return -1;
        }
        else if(a.month.toInt() > b.month.toInt())
        {
            return 1;
        }
        else
        {
            if(a.day.toInt() < b.day.toInt())
            {
                return -1;
            }
            else if(a.day.toInt() > b.day.toInt())
            {
                return 1;
            }
            else
            {
                if(a.hour.toInt() < b.hour.toInt())
                {
                    return -1;
                }
                else if(a.hour.toInt() > b.hour.toInt())
                {
                    return 1;
                }
                else
                {
                    if(a.minute.toInt() < b.minute.toInt())
                    {
                        return -1;
                    }
                    else if(a.minute.toInt() > b.minute.toInt())
                    {
                        return 1;
                    }
                    else
                    {
                        return 0;
                    }
                }
            }
        }
    }
}
void quicksort(activity *item,int begin,int end)
{
    if(begin > end)
    {
        return;
    }
    int i,j;
    activity temp;
    temp = item[begin];
    i = begin;
    j = end ;
    while(i < j)
    {
        while( i < j && activity_time_compare(item[j].get_start_time() , temp.get_start_time()) > 0)
        {
            j--;
        }
        item[i] = item[j];
        while( i < j && activity_time_compare(item[i].get_start_time() , temp.get_start_time()) <= 0 )
        {
            i++;
        }
        item[j] = item[i];
    }
    item[i] = temp;
    quicksort(item, begin, i-1);
    quicksort(item, i+1,end);
}//快速排序
