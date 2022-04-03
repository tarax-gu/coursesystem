#include "course.h"
#include "function_sort_and_search.cpp"
#include "function_upload_and_update.cpp"
course item[1000];
int main()
{
	int m;
	cout << "测试：输入0结束功能，输入1进入查找功能，输入2进入排序功能,输入3进入上传功能" << endl;
	cin >> m;
	while(m != 0)
	{
	   	if( m == 1)
	   	{
	   		cout<<"请输入课程名称:\n"; 
	   		search();
		}	
		else if( m == 2)
		{	
			int k,total = 0;
			cout<<"输入排序种类：1名称，2上课地点\n";
			cin >> k;
			ifstream in("_course_information.txt");
			while(read_course(in, item[total]))
			{
				total++;
			}
			in.close();
			quicksort(item,0,total-1,k);
			for(int i = 0 ; i < total ; i++)
			{
				print_course(cout, item[i]);
			}
		}
		else if(m == 3)
		{
			cout << "请选择上传的类型:"
				 << "输入1上传作业,输入2上传课程资料" << endl; 
			int k;
			cin >> k; 
			if(k == 1)
			{
				string coursename,homeworkname;
				cout << "请输入作业所属课程名称：\n";
				cin >> coursename;
				cout << "请输入作业名称：\n" ;
				cin >> homeworkname;
				upload_homework(coursename,homeworkname);
			}
			else if(k == 2)
			{
				string coursename,dataname;
				int year,month,day;
				cout << "请输入课程名称：\n" ;
				cin >> coursename; 
				cout << "请输入课程资料名称与上传日期：\n" ;
				cin >> dataname >> year >> month >> day;
				upload_data(coursename,dataname,year,month,day);	
			} 
		} 
		else
		{
			cout << "输入指令不正确\n" << endl;
		}
	   cout << "测试：输入0结束功能，输入1进入查找功能，输入2进入排序功能,输入3进入上传功能" << endl; 	
	   cin >> m;	
	}
	return 0; 
}
