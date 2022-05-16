#ifndef DB_MANAGE_H
#define DB_MANAGE_H
#include <QDir>
#include <QFileInfo>
#include <QFile>
#include <QDebug>
#include "ui_login.h"
#include "qdesktopservices.h"
#include <QUrl>
#include <qmessagebox.h>
#include <QDateTime>
#include <QVector>
struct dbstruct{
    QString dbname;
    QString path;
    QString createtime;
    QString dbtype;
};

class database_manage{


public:
    void createDatabase(QString dbname);
    void deleteDatabase(QString dbname,QWidget *tempUi);

    QVector <dbstruct*> dbvector;
};

#endif // DB_MANAGE_H
