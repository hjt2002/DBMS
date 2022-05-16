#include "db_powermanage.h"
db_power::db_power(){
}

void db_power::grantPower(QString str){

//    QStringList list = str.split(" ");
    //创建正则对象
    QString pattern("grant\\s+(\\w+)\\s+on\\s+table\\s+(\\w+)\\s+to\\s+(\\w+)");
    QRegExp rx(pattern);



    int pos = str.indexOf(rx);
//    qDebug() << pos;

    if(pos >= 0){
//        qDebug() <<  rx.capturedTexts();

        power = rx.cap(1);
        ptablename = rx.cap(2);
        pUsername = rx.cap(3);

    }

    writePower(global::getUsername(),global::getDBname(),ptablename,power);

}
void db_power::revokePower(QString str){

    QString pattern("revoke\\s+(\\w+)\\s+on\\s+table\\s+(\\w+)\\s+from\\s+(\\w+)");
    QRegExp rx(pattern);

    int pos = str.indexOf(rx);
//    qDebug() << pos;
    if(pos >= 0){
//        qDebug() <<  rx.capturedTexts();

        power = rx.cap(1);
        ptablename = rx.cap(2);
        pUsername = rx.cap(3);
    }

//    readPower();//读取信息到内存文件
    read(pUsername);
    QVector<powerStruct*>::iterator iter;
    for(iter = vector.begin();iter != vector.end();iter++){

        if(global::getUsername() == (*iter)->username && global::getDBname() == (*iter)->dbname
                && ptablename == (*iter)->tablename && power == (*iter)->power){

            //匹配对应
            vector.erase(iter);
            break;
        }

    }
    //重新写回文件
    //清空文件
    QFile tempfile(global::getfilepath().append("/" + pUsername + "/power.txt"));
    if(!tempfile.open(QIODevice::WriteOnly|QIODevice::Text|QIODevice::Truncate)){
        qDebug() << "文件清空失败!";
    }else {
        tempfile.close();
    }

    for(iter = vector.begin();iter != vector.end();iter++){
        if((*iter)->username != NULL){
            writePower((*iter)->username,(*iter)->dbname,(*iter)->tablename,(*iter)->power);
        }
    }
}

void db_power::read(QString pathname){
    QString path  = global::getfilepath();
    path.append("/" + pathname +"/" + "power.txt");
    QFile pfile(path);
    if(!pfile.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug() << "open power fail!";
        exit(0);
    }

    QTextStream in(&pfile);
    QString line;
    while(!in.atEnd()){
        line = in.readLine();
        if(!line.isEmpty()){
            QStringList list = line.split(" ");
            powerStruct *power = new powerStruct;
            power->username = list.at(0);
            power->dbname = list.at(1);
            power->tablename = list.at(2);
            power->power = list.at(3);
            vector.push_back(power);
        }

    }
    pfile.close();
}



//void db_power::readPower(){

//    //对应power文件
//    QString filepath = global::getfilepath();
//    filepath = filepath.append("/" + pUsername +"/power.txt");
//    QFile file(filepath);

//    QTextStream in(&file);
//    if(!file.open(QFile::ReadOnly)){
//        qDebug() << "power can't open!";
//    }else{

//        QString line = in.readLine();
//        QString line = file.readLine();
//        line.trimmed();

//        QStringList list =line.split(" ");
//        powerStruct *powerStr = new powerStruct();
//        powerStr->username = list.at(0);
//        powerStr->dbname = list.at(1);
//        powerStr->tablename = list.at(2);
//        powerStr->power = list.at(3);

//        vector.push_back(powerStr);
//        while(!line.isEmpty()){

//            line = file.readLine();
//            line.trimmed();
//            if(!line.isEmpty()){

//                QStringList list =line.split(" ");
//                powerStruct *powerStr = new powerStruct();
//                powerStr->username = list.at(0);
//                powerStr->dbname = list.at(1);
//                powerStr->tablename = list.at(2);
//                powerStr->power = list.at(3);

//                vector.push_back(powerStr);

//            }else{
//                qDebug() << "current line is null!power file to end!";
//                break;
//            }
//        }

//    }
//    file.close();
//}

void db_power::writePower(QString username,QString dbname,QString tablename,QString power){

    //权限写入对应用户的权限管理文件
    QString filepath = global::getfilepath().append("/");
    filepath = filepath.append(pUsername).append("/power.txt");
    QFile file(filepath);

    if(!file.open(QFile::WriteOnly|QFile::Append)){
        qDebug() << "power can't open!";
    }else{

        QString content = username + " " + dbname + " "+ tablename +" " + power + "\r\n";
        file.seek(file.size());
        file.write(content.toLatin1());
        file.close();
    }
}


