#ifndef DB_VIEW_H
#define DB_VIEW_H

#include "ui_mainwindow.h"

#include<iostream>
#include<fstream>
#include<QVector>
#include <regex>
#include "path_global.h"
using namespace std;
class db_view
{
public:
    db_view();


public:
//    string view_path="D:\\APP\\QT\\Code\\QTworkspace1\\view\\";//用于更改文件路径  ,如果用\分割要多加一个\转义
//    string table_path="D:\\APP\\QT\\Code\\QTworkspace1\\data\\";
     string view_path;
     string table_path;

    QTextBrowser * display; //主ui的展示窗口，方便调用

    string view_comment;//创建视图 的完整语句
    string view_table_name;//视图基于的基本表

    //关于基本表的信息
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


    QString s0="数据项： ";
    QString s1="关键字： ";
    QString s2="所选视图： ";
    QString s3="判断条件： ";

public:
    void set_dis(QTextBrowser *); //初始化
    void comment(QString s);//传入SQL语句

    int view_drop(char* );//删除视图
    int view_create(char*);//创建视图
    int view_select(char*);//查询视图(select* )
    int view_select_some(char *);//选取列查询

    bool read_view_File(string table_name);//读取视图内容
    int judge_type(string);//判断数据类型
    void judge_condition(string);//对判断条件解析
};

#endif // DB_VIEW_H
