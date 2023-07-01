#include"Mission.h"
#include<iostream>
#include<cstring>
#include<string>
#include<stdlib.h>
#include"CODEANDDECODE.h"
#include<time.h>
#include<queue>
#include <fstream>
#include <sstream>
using namespace std;

/*
待实现函数：
1.mission类里的所有函数
5.run（）函数（改）
logout函数
login修改有返回值
7.addTask()  showTask()  delTask()  clearTask()
*/


void synchronize(string fileplace, priority_queue<mission>& task_array)
{
	FILE* fp = fopen(fileplace.c_str(), "w");

	if (fp == NULL) {
		cout << "Error opening file\n";
		return;
	}

	mission a;
	while (!task_array.empty()) {
		a = task_array.top();
		task_array.pop();
		fputs(a.task_name.c_str(), fp);
		fputs(" ", fp);
		string time;
		time = to_string(a.do_time.year) + ":" + to_string(a.do_time.month) + ":" + to_string(a.do_time.day) + ":" + to_string(a.do_time.hour) + ":" + to_string(a.do_time.min) + ":" + to_string(a.do_time.sec);
		fputs(time.c_str(), fp);
		fputs(" ", fp);
		time = to_string(a.remind_time.year) + ":" + to_string(a.remind_time.month) + ":" + to_string(a.remind_time.day) + ":" + to_string(a.remind_time.hour) + ":" + to_string(a.remind_time.min) + ":" + to_string(a.remind_time.sec);
		fputs(time.c_str(), fp);
		fputs(" ", fp);
		fprintf(fp, "%c ", a.priority);
		fputs(a.category.c_str(), fp);
		fputs(" ", fp);
		fprintf(fp, "%d\n", a.ID);
	}
	return;
}

//注意每一个操作后都要同步到本地文件，输入的任务，要保存到本地文件；每一个任务输入完成后自动保存到文件；
void Complete_help() {
	cout << "完整的命令行帮助说明和使用范例\n";
	cout << "命令行帮助说明：\n";
	cout << "1.命令行参数：run\n";
	cout << "2.命令行参数：-h\n";
	cout << "3.命令行参数：addTask\n";
	cout << "4.命令行参数：showTask\n";
	cout << "5.命令行参数：delTask\n";
	cout << "6.命令行参数：clearTask\n";
	cout << "7.命令行参数：createuser\n";
	cout << "8.命令行参数：login\n";
	cout << "9.命令行参数：logout\n";
	cout << "8.命令行参数：exit\n";
}



void createuser()
{
	string username;
	string password;
	cout << "请输入用户名：";
	cin >> username;
	cout << "请输入密码：";
	cin >> password;
	//加密
	encode(&username, &password);
	//将加密后的用户名和密码写入文件
	FILE* fp;
	fp = fopen("user.txt", "a");
	if (fp == NULL)
	{
		cout << "打开文件失败！" << endl;
		exit(0);
	}
	fputs(username.c_str(), fp);
	fputs("\n", fp);
	fputs(password.c_str(), fp);
	fputs("\n", fp);
	fclose(fp);
	fp = fopen(username.c_str(), "w");
}

string login()
{
	string username;
	string password;
	cout << "请输入用户名：";
	cin >> username;
	cout << "请输入密码：";
	cin >> password;
	FILE* fp;
	fp = fopen("user.txt", "r");
	if (fp == NULL)
	{
		cout << "打开文件失败！" << endl;
		exit(0);
	}
	char cencode_username[100];
	char cencode_password[100];
	bool dismatch = 0;
	while (1)
	{
		fgets(cencode_username, 100, fp);
		fgets(cencode_password, 100, fp);
		if (cencode_username == 0)
		{
			dismatch = 1;
			break;
		}
		string encode_username = cencode_username;
		string encode_password = cencode_password;
		decode(&encode_username, &encode_password);
		return encode_username;
		if (encode_username == username && encode_password == password)
		{
			cout << "登录成功！" << endl;
			return encode_username;
		}
	}
	if (dismatch == 1)
	{
		cout << "用户名或密码错误！" << endl;
		login();
	}
	fclose(fp);
	
}
void FileInput(string fileplace, priority_queue<mission>& array_task) {
	ifstream file(fileplace);

	if (!file.is_open()) {
		printf("Error opening file\n");
		return;
	}

	while (!array_task.empty()) {
		array_task.pop();
	}

	mission a;
	int new_ID = 0;

	while (file >> a.task_name >> a.do_time.year >> a.do_time.month >> a.do_time.day >> a.do_time.hour >> a.do_time.min >> a.do_time.sec >> a.remind_time.year >> a.remind_time.month >> a.remind_time.day >> a.remind_time.hour >> a.remind_time.min >> a.remind_time.sec >> a.priority >> a.category >> a.ID) {
		array_task.push(a);
		if (a.ID > new_ID) new_ID = a.ID;
	}

	g_ID = new_ID;
	return;
}

void showTask(priority_queue<mission>task_array) {
	mission tmp;
	while (!task_array.empty()) {
		tmp = task_array.top();
		show(tmp);
		task_array.pop();
	}
}

void delTask(string fileplace, priority_queue<mission>& array_task, int delID) {
	priority_queue<mission> tmp;
	mission trans;

	while (array_task.top().ID != delID) {
		trans = array_task.top();
		array_task.pop();
		tmp.push(trans);
	}

	array_task.pop();

	while (!tmp.empty()) {
		trans = tmp.top();
		tmp.pop();
		array_task.push(trans);
	}

	synchronize(fileplace, array_task);
}

void clearTask(string fileplace, priority_queue<mission> &task_array)
{
	while (!task_array.empty()) {
		task_array.pop();
	}

	synchronize(fileplace, task_array);
}


void Stringsplit(string str, const char split, vector<string>& rst) {
	istringstream iss(str);
	string token;
	while (getline(iss, token, split)) {
		rst.push_back(token);
	}
}


void addTask(string fileplace, priority_queue<mission>& task_array) 
{
	string unknown[5];
	int cnt = 0;
	string time_do, time_remind;

	while (cin >> unknown[cnt]) {
		cnt++;
	}

	mission a;
	vector<string> data;

	a.task_name = unknown[0];

	Stringsplit(unknown[1], ':', data);
	a.do_time.sec = atoi(data.back().c_str());
	data.pop_back();
	a.do_time.min = atoi(data.back().c_str());
	data.pop_back();
	a.do_time.hour = atoi(data.back().c_str());
	data.pop_back();
	a.do_time.day = atoi(data.back().c_str());
	data.pop_back();
	a.do_time.month = atoi(data.back().c_str());
	data.pop_back();
	a.do_time.year = atoi(data.back().c_str());
	data.pop_back();

	priority_queue tmp;
        mission cek;
        tmp = task_array; 
        while(!tmp.empty()){
            cek = tmp.top();
            if(cek.task_name == a.task_name && time_cpr(cek.do_time,a.do_time) == 0){
            printf("Error:Already have the same task\n");
            return -1;
            }  
	tmp.pop();
        } 


	Stringsplit(unknown[2], ':', data);
	a.remind_time.sec = atoi(data.back().c_str());
	data.pop_back();
	a.remind_time.min = atoi(data.back().c_str());
	data.pop_back();
	a.remind_time.hour = atoi(data.back().c_str());
	data.pop_back();
	a.remind_time.day = atoi(data.back().c_str());
	data.pop_back();
	a.remind_time.month = atoi(data.back().c_str());
	data.pop_back();
	a.remind_time.year = atoi(data.back().c_str());
	data.pop_back();

	if (cnt == 3) { a.priority = 'm'; a.category = "others"; }

	else if (cnt == 4) {
		if (strcasecmp(unknown[3].c_str(), "m") == 0 || strcasecmp(unknown[3].c_str(), "l") == 0 || strcasecmp(unknown[3].c_str(), "h") == 0) {
			a.priority = unknown[3][0];
			a.category = "others";
		}
		else { a.priority = 'm'; a.category = unknown[3]; }
	}

	else if (cnt == 5) {
		if (strcasecmp(unknown[3].c_str(), "m") == 0 || strcasecmp(unknown[3].c_str(), "l") == 0 || strcasecmp(unknown[3].c_str(), "h") == 0) {
			a.priority = unknown[3][0];
			a.category = unknown[4];
		}
		else { a.priority = unknown[4][0]; a.category = unknown[3]; }
	}

	g_ID++;
	a.ID = g_ID;

	task_array.push(a);

	FILE* fp = fopen(fileplace.c_str(), "a");

	if (fp == NULL) {
		printf("Error opening file\n");
		return;
	}

	fputs(a.task_name.c_str(), fp);
	fputs(" ", fp);
	string time;
	time = to_string(a.do_time.year) + " " + to_string(a.do_time.month) + " " + to_string(a.do_time.day) + " " + to_string(a.do_time.hour) + " " + to_string(a.do_time.min) + " " + to_string(a.do_time.sec);
	fputs(time.c_str(), fp);
	fputs(" ", fp);
	time = to_string(a.remind_time.year) + " " + to_string(a.remind_time.month) + " " + to_string(a.remind_time.day) + " " + to_string(a.remind_time.hour) + " " + to_string(a.remind_time.min) + " " + to_string(a.remind_time.sec);
	fputs(time.c_str(), fp);
	fputs(" ", fp);
	fprintf(fp, "%c ", a.priority);
	fputs(a.category.c_str(), fp);
	fputs(" ", fp);
	fprintf(fp, "%d\n", a.ID);

	return;
}

void run(string fileplace, priority_queue<mission>& array_task) {
	string temp0;
	cout << "请输入命令: ";
	cin >> temp0;//这儿用temp初步接受命令
	const char *temp=nullptr;
	temp=temp0.c_str();
	if (strcasecmp(temp,"addTask")==0) {
		addTask(fileplace,array_task);
	}
	else if (strcasecmp(temp,"showTask")==0) {
		showTask(array_task);
	}
	else if (strcasecmp(temp,"delTask")==0) {
		cout << "please enter delID:";
		int delID;
		cin >> delID;
		delTask(fileplace, array_task, delID);
	}
	else if (strcasecmp(temp,"clearTask")==0) {
		clearTask(fileplace, array_task);
	}
	else if (strcasecmp(temp,"createUser")==0) {
		createuser();
	}
	else if (strcasecmp(temp,"login")==0) {
		login();
	}
	else if (strcasecmp(temp,"exit")==0) {
		exit(0);
	}
	else {
		cout << "命令错误，请重新输入！" << endl;
		run(fileplace, array_task);
	}
}
