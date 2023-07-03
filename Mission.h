#pragma once
#include<cstring>
#include<unistd.h>
#include<iostream>
#include<stdlib.h>
#include<pthread.h>
#include<vector>
#include<queue>
#include<ctime>
using namespace std;
/*可能会继续增加
logout退出登录功能
*/
int g_ID = 0;
Time get_reminding_time(mission subject) {
    return subject.remind_time;
}
struct Time {
    int year;
    int month;
    int day;
    int hour;
    int min;
    int sec;
    Time(int h = 0, int mi = 0, int s = 0, int y = 0, int mon = 0, int d = 0)
    {
        year = y;
        month = mon;
        day = d;
        hour = h;
        min = mi;
        sec = s;
    }
    ~Time() {};
};

class mission {
public:
    std::string task_name;//任务名称
    Time do_time;//任务开始时间
    Time remind_time;//提醒时间
    char priority;//优先级,这里用l,m,h代表优先级的低中高
    std::string category;//分类情况,分为"study","life","fun"代表学习、生活、娱乐
    int ID;//分配的ID

public:
    mission()
    {
        task_name = "0";
        do_time = 0;
        remind_time = 0;
        priority = 0;
        category = "0";
    }
    mission(std::string t_name, Time d_time, Time r_time, char pri = 'm', std::string cat = "unclassified") {
        //任务名和提醒时间必须输入，优先级和分类不输入就是空，启动时间获取系统时间自动生成，ID自动生成
        task_name = t_name;
        do_time = d_time;
        remind_time = r_time;
        priority = pri;
        category = cat;
        //ID赋值在addtask函数中实现，将新生成的misson的ID赋值为最后一个mission的ID+1
    };
    ~mission() { }
    friend int time_cpr(const Time& a, const Time& b);
    friend bool operator<(const mission& a, const mission& b);
};

int time_cpr(const Time& a, const Time& b) {//a的时间更晚返回1，a的时间更早返回-1，a和b同时等于返回0
    if (a.year > b.year) return 1;
    else if (a.year < b.year) return -1;
    else {
        int tmp1 = 0; int tmp2 = 0;
        tmp1 = 100 * a.month + a.day;
        tmp2 = 100 * b.month + b.day;
        if (tmp1 != tmp2) return (tmp1 > tmp2) ? 1 : -1;
        tmp1 = 3600 * a.hour + 60 * a.min + a.sec;
        tmp2 = 3600 * b.hour + 60 * b.min + b.sec;
        if (tmp1 != tmp2) return (tmp1 > tmp2) ? 1 : -1;
        return 0;//最后肯定只能等于
    }

}

bool operator<(const mission& a, const mission& b) {
    if (time_cpr(a.remind_time, b.remind_time) == 1) return true;//如果a更晚返回true;
    else if (time_cpr(a.remind_time, b.remind_time) == 0) {
        if (a.priority == 'h')return false;
        else if (a.priority == 'm') {
            if (b.priority == 'm' || b.priority == 'l')return false;
        }
        else return true;
    }
    else return false;
}

void show(const mission a)//三行依次打印 名字 优先级 类别 ； 建立时间 ； 提醒时间
{
    cout << a.task_name << " " << a.priority << " " << a.category << endl;
    printf("%02d:%02d:%02d %d/%d/%d\n", a.do_time.hour, a.do_time.min, a.do_time.sec, a.do_time.year, a.do_time.month, a.do_time.day);
    printf("%02d:%02d:%02d %d/%d/%d\n", a.remind_time.hour, a.remind_time.min, a.remind_time.sec, a.remind_time.year, a.remind_time.month, a.remind_time.day);
}
void createuser();//创建用户
void Complete_help();//展示完整的命令行帮助说明
string login();//登录函数输入用户名、口令，包含加密过程
void* check(void* arg);//子线程进行对提醒时间的检查,return NULL即可，不需要从子线程再获取数据
void FileInput(string fileplace, priority_queue<mission>& array_task);//读入本地文件，存储到task_arraay中
void run(string fileplace, priority_queue<mission>& array_task);//run了之后，开始执行各种命令的函数，各种命令都包含在这个大函数里面
void addTask(string fileplace, priority_queue<mission>& task_array);
//这个要注意每个任务的开始时间不同，任务名称+开始时间具有唯一性，任务优先级和类型有缺省值
//还有给任务分配唯一的ID
void showTask(priority_queue<mission>task_array);//注意自行设计显示方式
void delTask(string fileplace, priority_queue<mission>& array_task, int delID);//根据任务id，删除某个指定任务
void clearTask(string fileplace, priority_queue<mission>& task_array);//清空任务
void synchronize(string fileplace, priority_queue<mission>& task_array);//将本地文件和内存task_array中的任务同步*/
