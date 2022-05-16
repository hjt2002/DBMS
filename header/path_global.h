#ifndef PATH_GLOBAL_H
#define PATH_GLOBAL_H
#include  <QString>
#include <QFile>
#include <QDebug>



/*
 * this file is used to store the
 * path of username,dbname and so on;
*/

class global {

    global();

private:

    //静态全局变量
    static QString username;
    static QString dbname;
    static QString filepath;

public:
    //全局函数
    static void setUsername(QString username);
    static void setDBname(QString dbname);
    static void setfilepath(QString filepath);



    static QString getUsername();
    static QString getDBname();
    static QString getfilepath();


    static void writeToLog(QString log);//写到日志文件中
    static void readLog(QString path);
};


#endif // PATH_GLOBAL_H
