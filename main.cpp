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
using namespace std;

vector<mission>task_array;//读取本地文件中任务后，将任务存储在内存中的容器
pthread_t sub_thread_id;//子线程id
pthread_mutex_t mutex;//互斥锁

int main(int argc,char *argv[]){
    if(argc!=2){//参数个数不匹配
        cout<<"INPUT ERROR: Invalid parameters\n";//输入错误，参数无效
        exit(-1);
    }
    int result;//用来接收getopt的信息
    while((result=getopt(argc,argv,"h"))!=-1){
        if(result=='h'){
            Complete_help();//展示完整的命令行帮助说明和使用的范例
            exit(0);//由于还没开始run就展示帮助，故这里直接退出程序
        }
    }
    if(strcasecmp(argv[1],"run")==0){//如果参数为run，则启动程序
        pthread_create(&sub_thread_id,NULL,check,NULL);//
        login();//登录函数，包含口令加密过程
        //登录之后就导入本地文件
        pthread_mutex_init(&mutex,NULL);//互斥锁初始化
        pthread_mutex_lock(&mutex);//设置互斥锁
        FileInput();//进入存储任务的文件，将本地文件读入task_array中
        pthread_mutex_unlock(&mutex);//解锁
        
        while(true){
            run();
        } 
    }
    else {//参数不为run说明日程管理还未启动
        cout<<"Not Run Yet.\n";
        exit(0);
    }
}
