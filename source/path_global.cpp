#include "path_global.h"

//参数初始化static
QString global::username = "";
QString global::dbname = "";
QString global::filepath = "E:/QTworkspace/data";//文件路径

global::global(){

}


void global::setUsername(QString user){
    username = user;
}
void global::setDBname(QString db){
    dbname = db;
}
void global::setfilepath(QString file){
    filepath = file;
}

QString global::getfilepath(){

    return filepath;
}

QString global::getUsername(){
    return username;
}
QString global::getDBname(){
    return dbname;
}

//写入日志文件
void global::writeToLog(QString log){
        QString path = filepath;
        QString tempfilepath = path.append("/" + username +"/" + dbname);//数据库文件夹
        QString pathstr = tempfilepath.append("/" + dbname + ".log");
        QFile file(pathstr);
        if(!file.open(QIODevice::Append|QIODevice::Text)){
            qDebug() << "log file can't open";
        }else {
           qDebug() << "log added!";
           file.seek(file.size());
           file.write("\n");
           file.write(log.toLatin1());
           file.close();
        }
}
void global::readLog(QString path){
}
