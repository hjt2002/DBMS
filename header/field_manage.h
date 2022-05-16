#ifndef FIELD_MANAGE_H
#define FIELD_MANAGE_H
#include <QString>
#include <field_xml.h>
#include <fieldentity.h>
#include<iostream>
#include<fstream>
#include<QVector>
#include <regex>
#include <QTextBrowser>
#include "path_global.h"
using namespace std;
class field_manage
{
public:
//    QString filePath="C:/Users/L00I/Desktop/DBMS/data/field.xml";
    QString filePath = global::getfilepath().append("/field.xml");
    void addField(char *str,QTextBrowser *tbrowser);
    void deleteField(char *str,QTextBrowser *tbrowser);
    void modifyField(char *str,QTextBrowser *tbrowser);
    void read(QString fiename,QTextBrowser *tbrowser);
};

#endif // FIELD_MANAGE_H
