#include "db_manage.h"


void database_manage::createDatabase(QString dbname){
//创建数据库函数
//  QString folderPath = "E:/QTworkspace/data";//默认工作文件路径
    QString folderPath = global::getfilepath();//获取data文件路径

    folderPath.append("/"+ global::getUsername());
    folderPath.append("/" + dbname);

//  folderPath.append("/aaa");
    //创建用户文件夹
    QDir dir(folderPath);
    if(!dir.exists()){
        dir.mkdir(folderPath);
        qDebug() << "create database success!";
    }else{
        qDebug() << "file exists!";
    }

    //写入该数据库信息到数据库文件中ruanko.db
    QString temppath = global::getfilepath().append("/ruanko.db");
    QFile file(temppath);
    if(!file.open(QFile::WriteOnly|QFile::Append)){
        qDebug() << "ruanko.db open failed!";
    }else {
        //数据库信息文件打开！
        QDateTime current_time = QDateTime::currentDateTime();
        QString currentTime = current_time.toString("yyyy-MM-dd,hh:mm:ss");
        QString str =  dbname + " " + folderPath + " " + currentTime + " " + "user" + "\r\n";
        file.seek(file.size());
//        file.write("\n");
        file.write(str.toLatin1());
        file.close();
    }
}

void database_manage::deleteDatabase(QString dbname,QWidget *tempUi){

    //打开工作文件夹
    QString path = global::getfilepath();
//    QDesktopServices::openUrl(QUrl(path,QUrl::TolerantMode));

    path.append("/" + global::getUsername() + "/" + dbname);
    //delete database

    QDir dir;

    bool flag = false;
    if(!dir.exists(path)){
        qDebug() << "dir not found!";
    }else{
        dir.setPath(path);
        dir.setFilter(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);
        QFileInfoList list = dir.entryInfoList();
        //数据库中存在表
        if(list.count() <= 0){
            //database null delete!
            qDebug() << "file is null!Ready to delete!";
            flag = true;

         }else{
            //文件夹不为空
            qDebug() << "database not null!";
            QMessageBox::StandardButton reply;
            reply = QMessageBox::question(tempUi,"Options","database exists!choose delete or not",QMessageBox::Yes | QMessageBox::No);
            if(reply == QMessageBox::Yes){
                //允许删除
                flag = true;
            }else{
                qDebug() << "database still exists!";
            }
        }
    }

    if(flag){
        //删除
        dir.setPath(path);
        dir.removeRecursively();
        qDebug() << "database delete success!";



        //修改数据库定义文件信息
        QString path = global::getfilepath().append("/ruanko.db");
        QFile file(path);
        QTextStream in(&file);



        //读取文件
        if(!file.open(QFile::WriteOnly|QFile::ReadOnly)){
            qDebug() << "ruanko.db can't open!";
        }else{

            QString line  = in.readLine();
//            qDebug() << "94 "  + line;
//            line.trimmed();//去掉换行符
            QStringList linelist  = line.split(" ");
            dbstruct *dbs = new dbstruct();

            dbs->dbname = linelist.at(0);
            dbs->path = linelist.at(1);
            dbs->createtime = linelist.at(2);
            dbs->dbtype = linelist.at(3);

            dbvector.push_back(dbs);

            while(!(line.isNull())){

                QString line  = in.readLine();

                if(!(line.isNull())){

                    QStringList linelist = line.split(" ");
                    dbstruct *dbs = new dbstruct();

                    dbs->dbname = linelist.at(0);
                    dbs->path = linelist.at(1);
                    dbs->createtime = linelist.at(2);
                    dbs->dbtype = linelist.at(3);

                    dbvector.push_back(dbs);

                }else{
                    qDebug() << "current line is empty!ruanko.db file is at the end!";
                    break;
                }
            }

            file.close();//读取完毕


            //查询删除目标数据库
            QVector<dbstruct*>::iterator iter;
//            int count = 0;
            for(iter = dbvector.begin();iter != dbvector.end();iter++){
                if(dbname == (*iter)->dbname){
                    //目标存在
//                    dbvector.remove(count,1);
                    dbvector.erase(iter);
                    break;
                }
//                count ++;
            }


            //写入
            QFile file(global::getfilepath().append("/ruanko.db"));
            if(!file.open(QFile::WriteOnly)){
            }else{
                //重新写入文件
                for(iter = dbvector.begin();iter != dbvector.end();iter++){
                    QString tempstr  = (*iter)->dbname + " " +(*iter)->path + " "
                            +(*iter)->createtime + " "
                            +(*iter)->dbtype;

                    if(!(tempstr.isNull())){
                        file.write(tempstr.toLatin1());
                        file.write("\r\n");
                    }

                }
                file.close();
            }
        }
    }
}
