/*************************************************************************
	> File Name: forktest.cpp
	> Author: ma6174
	> Mail: ma6174@163.com 
	> Created Time: 2018年06月20日 星期三 16时35分08秒
 ************************************************************************/
#include<unistd.h>
#include<iostream>
using namespace std;
int main()
{
	int i;
	if(fork()==0)
	{
		for(i=i;i<1000;i++)
			cout<<"this is child process"<<endl;
	}else
	{
		for(i=1;i<1000;i++)
			cout<<"this is parent process"<<endl;
	}
	return 0;
}
