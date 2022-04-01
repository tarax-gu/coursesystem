void search()
{
	int flag = 0;//定义标识符以判断是否查询到对应课程 
	string goal;//需查询课程的名称 
	course item;
	cin >> goal;//读入课程名称 
    ifstream in("courseinformation.txt");
    while(read_course(in, item))
    {
    	if( item.course_name.compare(goal) == 0)
    	{
    		flag = 1;
			print_course(cout,item);
		}//若找到对应的 
	}
	in.close();
	if(!flag)
	{
		cout<<"no this course.\n";
	}
}
int course_compare(course a,course b,int k)
{
	int t;
	switch(k)
	{
		case 1:
		t = a.course_name.compare(b.course_name);
		break;//k为1时根据课程名称排序
		case 2:
		t = a.class_loc.campus.compare(b.class_loc.campus);//k为2时根据校区进行排序
		if( t == 0 )
		{
			t = a.class_loc.position.compare(b.class_loc.position);//校区相同时根据课程地点进行排序
		}
		break;
		case 3:
			
		break;
	
	}
	return t;
}//课程比较
void quicksort(course *item,int begin,int end,int k)
{
   if(begin > end)return;
   int i , j ;
   course temp;
   temp = item[begin];
   i = begin;
   j = end ;
   while(i < j)
   {
   	while( i < j && course_compare(item[j] , temp , k) > 0)j--;
   	item[i] = item[j];
   	while( i < j && course_compare(item[i] , temp , k) <= 0 )i++;
   	item[j] = item[i];   
   }
   item[i] = temp;
   quicksort(item, begin, i-1,k);
   quicksort(item, i+1,end, k);	
}//快速排序
