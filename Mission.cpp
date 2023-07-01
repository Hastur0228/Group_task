#include"Mission.h"
#include<iostream>
#include<cstring>
#include<string>
#include<stdlib.h>
#include"CODEANDDECODE.h"
#include<time.h>
#include<queue>
#include <fstream>
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
		time = to_string(a.do_time.year) + ":" + to_string(a.do_time.month) + ":" + to_string(a.do_time.day) + ":" + to_String(a.do_time.hour) + ":" + to_string(a.do_time.min) + ":" + to_string(a.do_time.sec);
		fputs(time.c_str(), fp);
		fputs(" ", fp);
		time = to_string(a.remind_time.year) + ":" + to_string(a.remind_time.month) + ":" + to_string(a.remind_time.day) + ":" + to_String(a.remind_time.hour) + ":" + to_string(a.remind_time.min) + ":" + to_string(a.remind_time.sec);
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
}

void login()
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
		if (encode_username == username && encode_password == password)
		{
			cout << "登录成功！" << endl;
			break;
		}
	}
	if (dismatch == 1)
	{
		cout << "用户名或密码错误！" << endl;
		login();
	}
	fclose(fp);
}
void FileInput(string fileplace, priority_queue<mission>&array_task) {
	ifstream file(fileplace);
	if (!file.is_open()) {
		printf("Error opening file\n");
		return;
	}

	while (!array_task.empty()) {
		array_task.pop();
	}

	mission a;

	while (file >> a.task_name >> a.do_time.year >> a.do_time.month >> a.do_time.day >> a.do_time.hour >> a.do_time.min >> a.do_time.sec >> a.remind_time.year >> a.remind_time.month >> a.remind_time.day >> a.remind_time.hour >> a.remind_time.min >> a.remind_time.sec >> a.priority >> a.category >> a.ID) {
		array_task.push(a);
	}

	return;
}

void run() {
	string temp;
	cout << "请输入命令: ";
	cin >> temp;//这儿用temp初步接受命令
	if (temp == "addTask") {
		addTask();
	}
	else if (temp == "showTask") {
		showTask();
	}
	else if (temp == "delTask") {
		delTask();
	}
	else if (temp == "clearTask") {
		clearTask();
	}
	else if (temp == "createuser") {
		createuser();
	}
	else if (temp == "login") {
		login();
	}
	else if (temp == "exit") {
		exit(0);
	}
	else {
		cout << "命令错误，请重新输入！" << endl;
		run();
	}
}
