void upload_data(string coursename,string dataname,int year,int month,int day)
{
	string temp = coursename;//暂时保存课程名称 
	coursename += "_coursedatalist.txt";
	ofstream out;
	out.open(coursename.c_str(),ofstream::app);//打开课程资料文件清单，将课程资料信息存入文件中 
	out << dataname << " " << year << " " << month << " " << day << endl;
	out.close();//关闭文件输出流 
	dataname = "coursedata/" + temp + "_" + dataname + ".txt";//进入课程资料文件夹储存课程资料
	out.open(dataname.c_str());//打开课程资料文件生成资料文件
	out.close(); 
}
void upload_homework(string coursename,string homeworkname)
{
	string temp = coursename;//暂时保存课程名称 
	coursename += "_homeworklist.txt";
	ofstream out;
	out.open(coursename.c_str(),ofstream::app);//打开作业文件清单，将作业信息存入文件中 
	out << "\n" << homeworkname << " 1" << endl;
	out.close();//关闭文件输出流 
	homeworkname = "homework/" + temp + "_" + homeworkname + ".txt";//进入课程资料文件夹储存课程资料
	out.open(homeworkname.c_str());//打开课程资料文件生成资料文件
	out.close(); 
} 
