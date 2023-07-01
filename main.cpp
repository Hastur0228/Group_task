/*
1.注意输入优先级还有分类情况是不区分大小写的问题
*/
#include "Mission.h"
#include<cstring>
#include<unistd.h>
#include<iostream>
#include<stdlib.h>
#include<pthread.h>
#include<vector>
#include<queue>
using namespace std;
string fileplace;
priority_queue<mission> task_array;//自定义成小根堆，堆顶是提醒时间靠前的
//读取本地文件中任务后，将任务存储在内存中的容器
pthread_t sub_thread_id;//子线程id
pthread_mutex_t mutex;//互斥锁
//子线程后台提醒
void* check(void* arg)
{
	while (1) {
		pthread_mutex_lock(&mutex);//下面过程存在对任务文件改变的可能，故上锁
		time_t now = time(0);
		struct tm* now_time;//tm来自time.h
		now_time = localtime(&now);
		mission top_task=task_array.top();
        Time top_task_time = top_task.get_remind_time();
		Time current_time = (now_time->tm_year+1900, now_time->tm_mon+1, now_time->tm_mday, now_time->tm_hour, now_time->tm_min, now_time->tm_sec);
		if (time_cpr(current_time, top_task_time) == 0) {//说明当前时间就是最早提醒时间，需要立刻提醒
			system("clear");
			cout << "！！！！！！！！！！！\n";
			cout << "您有任务已到达提醒时间\n";
			cout << "请合理安排你的时间\n";
			cout << "！！！！！！！！！！！\n";
			show(top_task);
			task_array.pop();
			synchronize(fileplace,task_array);
		}
		else if (time_cpr(current_time, top_task_time) == 1) {//当前时间晚于最早提醒时间了，说明之前预设的提醒时间存在错误
			//此时指出用户的提醒时间有设置错误现象
			cout << "！！！！！！！！！！！\n";
			cout << "您预设的提醒时间可能有误，任务已超时\n";
			cout << "！！！！！！！！！！！\n";
		}
		//否则就是，当前时间早于最早提醒时间时,尚且不用提醒
		pthread_mutex_unlock(&mutex);
		sleep(1);//帮助回到主线程
	}
	return NULL;
}
int main(int argc, char* argv[]) {
    if (argc != 2) {//参数个数不匹配
        cout << "INPUT ERROR: Invalid parameters\n";//输入错误，参数无效
        exit(-1);
    }
    int result;//用来接收getopt的信息
    while ((result = getopt(argc, argv, "h")) != -1) {
        if (result == 'h') {
            Complete_help();//展示完整的命令行帮助说明和使用的范例
            exit(0);//由于还没开始run就展示帮助，故这里直接退出程序
        }
    }
    if (strcasecmp(argv[1], "run") == 0) {//如果参数为run，则启动程序
		pthread_create(&sub_thread_id, NULL, check, NULL);//
		fileplace = login();//登录函数，包含口令加密过程
        //登录之后就导入本地文件
        pthread_mutex_init(&mutex, NULL);//互斥锁初始化
        pthread_mutex_lock(&mutex);//设置互斥锁
		FileInput(fileplace, task_array);//进入存储任务的文件，将本地文件读入task_array中
        pthread_mutex_unlock(&mutex);//解锁
        while (true) {
            run(fileplace,task_array);
        }
    }
    else {//参数不为run说明日程管理还未启动
        cout << "Not Run Yet.\n";
        exit(0);
    }
}
