#pragma once
#include<iostream>
#include<cstring>
using namespace std;
/*可能会继续增加
logout退出登录功能
*/

class mission{
private:
    string task_name;//任务名称
    string boot_time;//任务启动时间
    char priority;//优先级,这里用l,m,h代表优先级的低中高
    string category;//分类情况,分为"study","life","fun"代表学习、生活、娱乐
    string reminding_time;//提醒时间
    int ID;//分配的ID
public:
    mission();//注意缺省值（优先级，类型有缺省值）的设置
    ~mission();
};

void createuser();//创建用户
void Complete_help();//展示完整的命令行帮助说明
void login();//登录函数输入用户名、口令，包含加密过程
void* check(void *arg);//子线程进行对提醒时间的检查,return NULL即可，不需要从子线程再获取数据
void FileInput();//读入本地文件，存储到task_arraay中
void run();//run了之后，开始执行各种命令的函数，各种命令都包含在这个大函数里面
void addTask();
//这个要注意每个任务的开始时间不同，任务名称+开始时间具有唯一性，任务优先级和类型有缺省值
//还有给任务分配唯一的ID
void showTask();//注意自行设计显示方式
void delTask();//根据任务id，删除某个指定任务
void clearTask();//清空任务
void synchronize();//将本地文件和内存task_array中的任务同步

