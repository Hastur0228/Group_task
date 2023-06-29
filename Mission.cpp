#include"Mission.h"
#include<iostream>
#include<cstring>
#include<stdlib.h>
#include"CODEANDDECODE.h"
#include<time.h>
#include<queue>
using namespace std;

/*
待实现函数：
1.mission类里的所有函数
5.run（）函数（改）
logout函数
login修改有返回值
7.addTask()  showTask()  delTask()  clearTask()
*/


void synchronize(string fileplace,priority_queue<mission> &task_array)
{
        FILE *fp = fopen(fileplace,"w");

        if(fp == NULL){
                cout<<"Error opening file\n";
                return -1;}

        mission a;
        while(!task_array.empty()){
             a = task_array.top();
             task_array.pop();
             fputs(a.task_name,fp);
             fputs(" ",fp);
             string time;
             time = to_string( a.do_time.year) + ":" + to_string(a.do_time.month) + ":" +to_string(a.do_time.day)+":"+to_String(a.do_time.hour)+":"+to_string(a.do_time.min)+":"+to_string(a.do_time.sec);
             fputs(time,fp);
             fputs(" ",fp);
              time = to_string( a.remind_time.year) + ":" + to_string(a.remind_time.month) + ":" +to_string(a.remind_time.day)+":"+to_String(a.remind_time.hour)+":"+to_string(a.remind_time.min)+":"+to_string(a.remind_time.sec);
             fputs(time,fp);
             fputs(" ",fp);
             fprintf(fp,"%c ",a.priority);
             fputs(a.category,fp);
             fputs(" ",fp);
             fprintf(fp,"%d\n",a.ID);
        }
        return 0;
}
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
	cout<<"7.命令行参数：createuser\n";
	cout<<"8.命令行参数：login\n";
	cout<<"9.命令行参数：logout\n";
	cout<<"8.命令行参数：exit\n";
}

//子线程后台提醒
void *check(void *arg){
	while(1){
		pthread_mutex_lock(&mutex);//下面过程存在对任务文件改变的可能，故上锁
		time_t now=time(0);
    	struct tm* now_time;//tm来自time.h
    	now_time=localtime(&now);    
		Time top_task_time=task_array.top().get_reminding_time();
		Time current_time=(now_time->tm_year,now_time->tm_mon,now_time->tm_mday,now_time->tm_hour,now_time->tm_min,now_time->tm_sec);
		if(time_cpr(current_time,top_task_time)==2){//说明当前时间就是最早提醒时间，需要立刻提醒
			system("clear");
			cout<<"！！！！！！！！！！！\n";
			cout<<"您有任务已到达提醒时间\n";
			cout<<"请合理安排你的时间\n";
			cout<<"！！！！！！！！！！！\n";
			task_array.top().show();
			task_array.pop();
			synchronize();
		}
		else if(time_cpr(current_time,top_task_time)==1){//当前时间晚于最早提醒时间了，说明之前预设的提醒时间存在错误
        	//此时指出用户的提醒时间有设置错误现象
			cout<<"！！！！！！！！！！！\n";
        	cout<<"您预设的提醒时间可能有误，任务已超时\n";
			cout<<"！！！！！！！！！！！\n";
		}
		//否则就是，当前时间早于最早提醒时间时,尚且不用提醒
		pthread_mutex_unlock(&mutex);
		sleep(1);//帮助回到主线程
	}
	return NULL;
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
	fp=fopen("user.txt","a");
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
	else if(strcasecmp(cmd,"quit")){
		exit(0);
	}
	else {
		cout<<"Can't Find Your Commmand\n";
	}
}
