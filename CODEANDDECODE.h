#pragma once﻿
#include <iostream>
#include<stdio.h>
#include<string.h>
#include <unistd.h>

using namespace std;

int key[] = { 1,1,4,5,1,4,8 };

void encode(string& c) {
	int len = c.size();
	for (int i = 0; i < len; i++) {
		c[i] = c[i] ^ key[i % 7];
	}
}
void decode(string& c) {
	int len = c.size();
	for (int i = 0; i < len; i++) {
		c[i] = c[i] ^ key[i % 7];
	}
}
