#include"Mission.h"
#include<iostream>
#include<cstring>
#include<stdlib.h>
#include"CODEANDDECODE.h"
using namespace std;
/*
待实现函数：
1.mission类里的所有函数
2.void Complete_help();//展示"完整"的命令行帮助说明以及使用范例
3.login()函数
4.void* check()//子线程函数
4*.子线程中的检测函数和打印提醒
5.run（）函数
6.FileInput函数
7.addTask()  showTask()  delTask()  clearTask()
8.synchronize()将本地任务和内存中的任务同步
*/

//注意每一个操作后都要同步到本地文件，输入的任务，要保存到本地文件；每一个任务输入完成后自动保存到文件；
void Complete_help() {
	cout<<"完整的命令行帮助说明和使用范例\n";
	cout<<"命令行帮助说明：\n";
	cout<<"1.命令行参数：run\n";
	cout<<"2.命令行参数：-h\n";
	cout<<"3.命令行参数：addTask\n";
	cout<<"4.命令行参数：showTask\n";
	cout<<"5.命令行参数：delTask\n";
	cout<<"6.命令行参数：clearTask\n";
	cout<<"7.命令行参数：logout\n";
	cout<<"8.命令行参数：exit\n";
	cout<<"使用范例：\n";
	cout<<"1.命令行参数：run\n";
	cout<<"2.命令行参数：run -h\n";
	cout<<"3.命令行参数：run addTask\n";
	cout<<"4.命令行参数：run showTask\n";
	cout<<"5.命令行参数：run delTask\n";
	cout<<"6.命令行参数：run clearTask\n";
	cout<<"7.命令行参数：run logout\n";
	cout<<"8.命令行参数：run exit\n";
}

void createuser()
{
	string username;
	string password;
	cout<<"请输入用户名：";
	cin>>username;
	cout<<"请输入密码：";
	cin>>password;
	//加密
	encode(username, password);
	//将加密后的用户名和密码写入文件
	FILE *fp;
	fp=fopen("user.txt","w");
	if (fp == NULL)
	{
		cout<<"打开文件失败！"<<endl;
		exit(0);
	}
	fputs(username.c_str(), fp);
	fputs("\n",fp);
	fputs(password.c_str(), fp);
	fputs("\n",fp);
	fclose(fp);
}
void login()
{
	string username;
	string password;
	cout<<"请输入用户名：";
	cin>>username;
	cout<<"请输入密码：";
	cin>>password;
	FILE* fp;
	fp=fopen("user.txt","r");
	if (fp == NULL)
	{
		cout<<"打开文件失败！"<<endl;
		exit(0);
	}
	char encode_username[100];
	char encode_password[100];
	bool dismatch = 0;
	while (1)
	{
		fgets(encode_username,100,fp);
		fgets(encode_password,100,fp);
		if (encode_username == 0)
		{
			dismatch = 1;
			break;
		}
		decode(encode_username, encode_password);
		if (strcmp(encode_username, username.c_str()) == 0 && strcmp(encode_password, password.c_str()) == 0)
		{
			cout<<"登录成功！"<<endl;
			encode(encode_username,encode_password);
			FileInput(encode_username);
			break;
		}
	}
	if (dismatch == 1)
	{
		cout<<"用户名或密码错误！"<<endl;
		login();
	}
	fclose(fp);
}
void run(){
    string temp;
    cout<<"请输入命令: ";
    cin>>temp;//这儿用temp初步接受命令
    const char *cmd=nullptr;
    cmd=temp.c_str();
    if(strcasecmp(cmd,"addTask")){
        addTask();
    }
    else if(strcasecmp(cmd,"showTask")){
        showTask();
    }
    else if(strcasecmp(cmd,"delTask")){
        delTask();//根据任务id删除任务
    }
    else if(strcasecmp(cmd,"clearTask")){
        clearTask();
    }
}
