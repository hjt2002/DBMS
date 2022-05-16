#ifndef DB_XML_H
#define DB_XML_H
#include <QFile>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QtXml/QDomDocument>
#include <QDebug>
#include <QVector>
#include "path_global.h"


struct reg_inf{
    QString id;
    QString account;
    QString password;
};

class xml_inf{

public:
    //user xml
    void read_user_xml(QString user_dir);
    void write_user_xml(QString user_dir,QString account,QString passoword);


public:
    //储存用户信息的向量
    QVector <reg_inf*> reg_user_vector;

};
#endif // DB_XML_H
