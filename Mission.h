#pragma once
#include<ctime>
#include<iostream>
#include<cstring>

/*可能会继续增加
logout退出登录功能
*/
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
    ~Time();
};

class mission {
private:
    std::string task_name;//任务名称
    Time boot_time;//任务创建时间
    Time do_time;//任务开始时间
    Time remind_time;//提醒时间
    char priority;//优先级,这里用l,m,h代表优先级的低中高
    std::string category;//分类情况,分为"study","life","fun"代表学习、生活、娱乐
    int ID;//分配的ID
    int state;//任务被创建后为1，任务被删除后为0
    Time now() {//返回反映现在时间的Time类
        time_t timep;
        struct tm* p;
        time(&timep); //获取从1970至今过了多少秒，存入time_t类型的timep
        p = localtime(&timep);//用localtime将秒数转化为struct tm结构体
        //存储格式：时 分 秒 年 月 日 
        Time tmp_t(p->tm_hour, p->tm_min, p->tm_sec, 1900 + p->tm_year, 1 + p->tm_mon, p->tm_mday);
        return tmp_t;
    }
public:
    mission(std::string t_name, Time d_time, Time r_time, char pri = 'm', std::string cat = "unclassified") {
        //任务名和提醒时间必须输入，优先级和分类不输入就是空，启动时间获取系统时间自动生成，ID自动生成
        task_name = t_name;
        do_time = d_time;
        remind_time = r_time;
        priority = pri;
        category = cat;
        state = 1;

        time_t timep;
        struct tm* p;
        time(&timep); //获取从1970至今过了多少秒，存入time_t类型的timep
        p = localtime(&timep);//用localtime将秒数转化为struct tm结构体
        //存储格式：时 分 秒 年 月 日 
        Time tmp_t(p->tm_hour, p->tm_min, p->tm_sec, 1900 + p->tm_year, 1 + p->tm_mon, p->tm_mday);
        boot_time = tmp_t;
        //ID赋值在addtask函数中实现，将新生成的mission的ID赋值为最后一个mission的ID+1
    };
    void show()//三行依次打印 名字 优先级 类别 ； 建立时间 ； 提醒时间
    {
        printf("%s %c %s\n", task_name, priority, category);
        printf("%02d:%02d:%02d %d/%d/%d\n", boot_time.hour, boot_time.min, boot_time.sec, boot_time.year, boot_time.month, boot_time.day);
        printf("%02d:%02d:%02d %d/%d/%d\n", remind_time.hour, remind_time.min, remind_time.sec, remind_time.year, remind_time.month, remind_time.day);
    }
    void print_remind() {//打印提醒
        printf("REMINDING: you need to do the task:%s", task_name);
    }
    ~mission() { state = 0; }//将状态设置为被删除
    friend int time_cmp(const Time& a, const Time& b);
};

int time_cmp(const Time& a, const Time& b) {//a的时间更晚返回1，a的时间更早返回0，a和b同时等于返回2
    if (a.year > b.year) return 1;
    else if (a.year < b.year) return 0;
    else  {
        int tmp1 = 0; int tmp2 = 0;
        tmp1 = 100 * a.month + a.day;
        tmp2 = 100 * b.month + b.day;
        if (tmp1 != tmp2) return (tmp1 > tmp2) ? 1 : 0;
        tmp1 = 3600 * a.hour + 60 * a.min + a.sec;
        tmp2 = 3600 * b.hour + 60 * b.min + b.sec;
        if (tmp1 != tmp2) return (tmp1 > tmp2) ? 1 : 0;
        return 2;//最后肯定只能等于
    }
    
}

void createuser();//创建用户
void Complete_help();//展示完整的命令行帮助说明
void login();//登录函数输入用户名、口令，包含加密过程
void* check(void *arg);//子线程进行对提醒时间的检查,return NULL即可，不需要从子线程再获取数据
void FileInput(string fileplace);//读入本地文件，存储到task_arraay中
void run();//run了之后，开始执行各种命令的函数，各种命令都包含在这个大函数里面
void addTask();
//这个要注意每个任务的开始时间不同，任务名称+开始时间具有唯一性，任务优先级和类型有缺省值
//还有给任务分配唯一的ID
void showTask();//注意自行设计显示方式
void delTask();//根据任务id，删除某个指定任务
void clearTask();//清空任务
void synchronize();//将本地文件和内存task_array中的任务同步
