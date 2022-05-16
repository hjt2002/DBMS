#ifndef DB_DATA_H
#define DB_DATA_H

#include "ui_mainwindow.h"

#include "db_table.h"
#include<iostream>
#include<fstream>
#include<QVector>
#include <regex>
#include <QDatetime>
#include "db_powermanage.h"
using namespace std;

class db_data
{
public:
    db_data();

public:

//    string path="D:\\APP\\QT\\Code\\QTworkspace1\\data\\";//用于更改文件路径  ,如果用\分割要多加一个\转义

//      string path = "E:\\QTworkspace\\data\\admin\\test\\";
    string path;
//    db_table table_manager;
    QTextBrowser * display; //主ui的展示窗口，方便调用

    int num; //读取num
    QVector<string> attributes; //读取属性名和数据类型
    QVector<string> type;       //
    int count; //记录数量
    QVector<string> data_string;
    QVector<int> data_int;

    string a_string;//临时存储string变量
    int a_int;//临时存储int变量


    //判断条件的列和值
    string column;
    string value;


    static QString oname;

public:

    void set_dis(QTextBrowser *); //初始化
    void comment(QString s);//传入SQL语句
    void isKey(char *);

    bool readFile(string table_name,QString opname);//读取文件内容

    int data_select(char* p);//查询（默认select*）
    int data_select_some(char *p);//选取列查询
    int data_insert(char* p);//插入
    int data_update(char* p);//更新
    int data_delete(char* p);//删除
    int data_describe(char* p);//描述

    int judge_type(string);//判断数据类型
    void judge_condition(string);//对判断条件解析

};

#endif // DB_DATA_H
