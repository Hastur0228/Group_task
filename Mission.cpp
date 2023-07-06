#include"Mission.h"
#include<iostream>
#include<cstring>
#include<string.h>
#include<stdlib.h>
#include"CODEANDDECODE.h"
#include<time.h>
#include<queue>
#include <fstream>
#include<strings.h>
#include <sstream>
#include<stdio.h>
using namespace std;

/*Problem1:
1.用户名不能有空格了这里
2.输入时间的格式优化，并且要给出示例
*/

extern pthread_mutex_t mutex;
extern pthread_t sub_thread_id;

int time_cpr(const Time& a, const Time& b)
{//a的时间更晚返回1，a的时间更早返回-1，a和b同时等于返回0
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

bool operator<(const mission& a, const mission& b)
{
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

Time get_reminding_time(mission subject) {
	return subject.remind_time;
}


void show(const mission a)//三行依次打印 名字 优先级 类别 ； 建立时间 ； 提醒时间
{
	if (a.is_deleted == 0)
	{
		cout << "id:" << a.ID << " name:" << a.task_name << " priority:" << a.priority << " category:" << a.category << endl;
		printf("do time: %02d:%02d:%02d %d/%d/%d\n", a.do_time.hour, a.do_time.min, a.do_time.sec, a.do_time.year, a.do_time.month, a.do_time.day);
		printf("remind time: %02d:%02d:%02d %d/%d/%d\n", a.remind_time.hour, a.remind_time.min, a.remind_time.sec, a.remind_time.year, a.remind_time.month, a.remind_time.day);
	}
}

/*string to_string(int a)
{
	ostringstream ostr;
	ostr << a;
	string astr = ostr.str();
	//cout << astr <<endl;
	return astr;
}*/

void synchronize(string fileplace, priority_queue<mission>& task_array)
{
	FILE* fp = fopen(fileplace.c_str(), "w");
	if (fp == NULL) {
		cout << "Error opening file\n";
		return;
	}
	priority_queue<mission> tmp;
	mission a;
	while (!task_array.empty()) {
		a = task_array.top();
		task_array.pop();
		if (a.is_deleted == 0)
		{
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
		}
		tmp.push(a);
	}
	while (!tmp.empty()) {
		a = tmp.top();
		tmp.pop();
		task_array.push(a);
	}
	fclose(fp);
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
	char username_x[100];
	char password_x[100];
	cout << "请输入用户名：";
	cin >> username_x;
	cout << "请输入密码：";
	cin >> password_x;
	//加密
	string username = username_x;
	string password = password_x;
	encode(username);
	encode(password);
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
	decode(username);
	username += ".txt";
	fp = fopen(username.c_str(), "w");
	fclose(fp);
}

string login()
{
	string username;
	string password;
	cout << "请输入用户名：";
	cin >> username;
	username += '\n';
	cout << "请输入密码：";
	cin >> password;
	password += '\n';
	//加密
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
	while (!feof(fp))
	{
		fgets(cencode_username, 100, fp);
		fgets(cencode_password, 100, fp);
		string encode_username = cencode_username;
		string encode_password = cencode_password;
		decode(encode_username);
		decode(encode_password);
		if (encode_username == username && encode_password == password)
		{
			cout << "登录成功！" << endl;
			encode_username += ".txt";
			int i, j;
			for (i = 0, j = 0; i < encode_username.length(); i++)
			{
				if (encode_username[i] != '\n')
					cencode_username[j++] = encode_username[i];
			}
			cencode_username[j] = '\0';
			fclose(fp);
			return cencode_username;
		}
	}
	cout << "用户名或密码错误！" << endl;
	fclose(fp);
	return login();

}
void FileInput(string fileplace, priority_queue<mission>& array_task) {

	ifstream file(fileplace.c_str());

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
	priority_queue<mission> tmp;
	mission trans;
	while (!task_array.empty()) {
		trans = task_array.top();
		show(trans);
		task_array.pop();
		tmp.push(trans);
	}
	while (!tmp.empty()) {
		trans = tmp.top();
		tmp.pop();
		task_array.push(trans);
	}
}

void delTask(string fileplace, priority_queue<mission>& array_task, int delID) {
	priority_queue<mission> tmp;
	mission trans;
	pthread_mutex_lock(&mutex);
	while (!array_task.empty())
	{
		trans = array_task.top();
		if (array_task.top().ID == delID)
			trans.is_deleted = 1;
		array_task.pop();
		tmp.push(trans);
	}

	while (!tmp.empty()) {
		trans = tmp.top();
		tmp.pop();
		array_task.push(trans);
	}

	synchronize(fileplace, array_task);
	pthread_mutex_unlock(&mutex);
}

void clearTask(string fileplace, priority_queue<mission>& task_array)
{
	mission tep;
	priority_queue<mission> temp;
	pthread_mutex_lock(&mutex);
	while (!task_array.empty()) {
		tep = task_array.top();
		tep.is_deleted = 1;
		temp.push(tep);
		task_array.pop();
	}
	while (!temp.empty()) {
		tep = temp.top();
		temp.pop();
		task_array.push(tep);
	}
	synchronize(fileplace, task_array);
	pthread_mutex_unlock(&mutex);
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
	string time_do, time_remind;

	int cnt = 0;
	string unknown[5];
	while (cin >> unknown[cnt]) {
		cnt++;
		if (cnt > 5)
		{
			cout << "error input" << endl;
			addTask(fileplace, task_array);
		}
		char ch = cin.get();
		if (ch == '\n') break;
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

	priority_queue<mission>tmp;
	mission cek;
	tmp = task_array;
	while (!tmp.empty()) {
		cek = tmp.top();
		if (cek.task_name == a.task_name && time_cpr(cek.do_time, a.do_time) == 0 && cek.is_deleted == 0) {
			printf("Error:Already have the same task\n");
			return;
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

	pthread_mutex_lock(&mutex);
	task_array.push(a);
	synchronize(fileplace, task_array);
	pthread_mutex_unlock(&mutex);

}


void run(string fileplace, priority_queue<mission>& array_task) {
	string temp0;
	cout << "请输入命令: ";
	cin >> temp0;//这儿用temp初步接受命令
	const char* temp = NULL;
	temp = temp0.c_str();
	if (strcasecmp(temp, "addTask") == 0) {
		addTask(fileplace, array_task);
	}
	else if (strcasecmp(temp, "showTask") == 0) {
		showTask(array_task);
	}
	else if (strcasecmp(temp, "delTask") == 0) {
		cout << "please enter delID:";
		int delID;
		cin >> delID;
		delTask(fileplace, array_task, delID);
	}
	else if (strcasecmp(temp, "clearTask") == 0) {
		clearTask(fileplace, array_task);
	}
	else if (strcasecmp(temp, "exit") == 0) {
		exit(0);
	}
	else {
		cout << "命令错误，请重新输入！" << endl;
	}
	run(fileplace, array_task);
}
