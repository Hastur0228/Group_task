#pragma once﻿
#include <iostream>
/*
C:
    FILE*  fopen  fclose fread fscanf fgets fwrite fprintf fputs fseek ftell rewind
    r:只读 w:只写(新空文件) a:追加(尾部续写) r+:读写(必须存在) w+:读写(create) a+:读写(读取追加)(+b二进制形式）
C++:
    fstream open close read write get put seek
windows:

linux:

 */
#include<stdio.h>
#include <unistd.h>
void encode(string* username, string* password)//coding
{

    username ^= password;
    password ^= username;
    return;
}
void decode(string* username, string* password)//decoding
{
    password ^= username;
    username ^= password;
    return;
}
