#include"course_defination.h"
#include"function1_search.cpp"
#include"function1_sort.cpp"
course item[1000];
int main()
{
	int m;
	cout<<"测试：输入0结束，输入1查找，输入2排序"<<endl;
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
			ifstream in("courseinformation.txt");
			while(read_course(in, item[total]))total++;
			in.close();
			quicksort(item,0,total-1,k);
			for(int i = 0 ; i < total ; i++)
			{
				print_course(cout, item[i]);
				cout<<endl;
			}
		}
	   cout<<"测试：输入0结束，输入1查找，输入2排序"<<endl; 	
	   cin >> m;	
	} 
    
}
