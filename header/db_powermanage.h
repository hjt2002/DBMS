#ifndef DB_POWERMANAGE_H
#define DB_POWERMANAGE_H
#include <QString>
#include <QRegExp>
#include <QVector>
#include <QFile>
#include <QTextStream>
#include "path_global.h"

struct powerStruct{
    QString username;
    QString dbname;
    QString tablename;
    QString power;
};

class db_power{

//    Q_OBJECT

public:
    db_power();
//    ~db_power();
public:

    //被授权用户信息
    QString pUsername;
    QString ptablename;
    QString power;

    QVector <powerStruct*> vector;

public:

   void grantPower(QString str);
   void revokePower(QString str);


//   void readPower();
   void read(QString pathname);


   void writePower(QString username,QString dbname,QString tablename,QString power);

};



#endif // DB_POWERMANAGE_H
