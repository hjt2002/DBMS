#ifndef FIELD_XML_H
#define FIELD_XML_H
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QtXml/QDomDocument>
#include <QDebug>
#include <QVector>
#include <fieldentity.h>
#include <QTextBrowser>
struct def_inf{
    int id;
    QString name;
    int order;
    int type;
    int length;
    double max;
    double min;
    QString Default;
    int isPK;
    int isNull;
    int isUnique;
    QString comment;
};

class fie_inf{

public:
    void create_XML(QString filePath);//初始化xml文件
    void add_XML(QString filePath,QString table,QString column,QString type,QTextBrowser *tBrowser);//向xml文件中写入字段
    void modify_XML(QString filePath,QString table,QString column,QString type,QTextBrowser *tBrowser);//查询xml文件修改字段
    void drop_XML(QString filePath,QString table,QString column,QTextBrowser *tBrowser);//查询xml文件删除字段
public:
    //储存字段信息的向量
    QVector <def_inf*> def_field_vector;

};

#endif // FIELD_XML_H
