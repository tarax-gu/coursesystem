void search(void)
{
	int flag = 1;
	string goal;
	course item;
	cin >> goal;
    ifstream in("courseinformation.txt");
    while(read_course(in, item))
    {
    	if( item.course_name.compare(goal) == 0)
    	{
    		print_course(cout,item);
		}
	}
	if(flag == 0)cout<<"no this course.\n";
}

