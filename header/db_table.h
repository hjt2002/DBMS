#ifndef DB_TABLE_H
#define DB_TABLE_H

#include "ui_mainwindow.h"

#include<iostream>
#include<fstream>
#include<QVector>
#include <regex>
#include <stdio.h>
#include "path_global.h"

using namespace std;

class db_table
{
public:
    db_table();

public:
//    string path="D:\\APP\\QT\\Code\\QTworkspace1\\data\\";//用于更改文件路径  ,如果用\分割要多加一个\转义
    string path;


    QTextBrowser * display;
    QVector<string> attributes; //读取属性名和数据类型
    QVector<string> type;       //



    void set_dis(QTextBrowser *); //初始化
    void comment(QString s);//传入SQL语句
    int judge_type(string);//判断数据类型

    int table_create(char*);//创建表
    int table_drop(char*);//删除表
};



#endif // DB_TABLE_H
