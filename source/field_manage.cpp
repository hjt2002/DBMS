#include "field_manage.h"

void field_manage::read(QString fiename,QTextBrowser *display){
    QByteArray ba=fiename.toLatin1();
    char *str=ba.data();
    //通过正则表达式分辨不同sql语句
    if(regex_match(str, regex("alter +table +\\w+ +add +column +\\w+ +\\w+(\\(\\d*\\))?;*")))//增加字段语句
    {
        display->append("add field comment");
        qDebug()<<"<add field comment>";
        addField(str,display);
    }
    else if(regex_match(str, regex("alter +table +\\w+ +modify +column +\\w+ +\\w+(\\(\\d*\\))?;*")))//修改字段语句
    {
        display->append("modify field comment");
        qDebug()<<"<modify field comment>";
        modifyField(str,display);
    }
    else if(regex_match(str, regex("alter +table +\\w+ drop +column +\\w+;*")))//删除字段语句
    {
        display->append("delete field comment");
        qDebug()<<"<delete field comment>";
        deleteField(str,display);
    }
    else//未得到正确sql语句
    {
        display->append("please use right sentence");
        qDebug()<<"please use right sentence";
    }
}

void field_manage::addField(char *str,QTextBrowser *display){
    string tableName;
    string columnName;
    string typeName;
    smatch result;
    string s=str;

    //提取关键字
    regex Exp("alter +table +(\\w+) +add +column +(\\w+) +(\\w+(\\(\\d*\\))?);*");
    regex_match(s,result,Exp);
    tableName=result[1];
    columnName=result[2];
    typeName=result[3];
    if(typeName.compare("int")!=0&&typeName.compare("string")!=0){
        display->append("wrong type,please use int or string");
        qDebug()<<"wrong type,please use int or string";
        return;
    }
    qDebug()<<"table="<<QString::fromStdString(tableName)<<"column"<<QString::fromStdString(columnName)<<"type"<<QString::fromStdString(typeName);

    //向xml文件增加字段
    fie_inf fi;
    fi.add_XML(filePath,QString::fromStdString(tableName),QString::fromStdString(columnName),QString::fromStdString(typeName),display);
}

void field_manage::deleteField(char *str,QTextBrowser *display){
    string tableName;
    string columnName;
    smatch result;
    string s=str;

    //提取关键字
    regex Exp("alter +table +(\\w+) drop +column +(\\w+);*");
    regex_match(s,result,Exp);
    tableName=result[1];
    columnName=result[2];
    qDebug()<<"table="<<QString::fromStdString(tableName)<<"column"<<QString::fromStdString(columnName);

    //向xml文件增加字段
    fie_inf fi;
    fi.drop_XML(filePath,QString::fromStdString(tableName),QString::fromStdString(columnName),display);
}

void field_manage::modifyField(char *str,QTextBrowser *display){
    string tableName;
    string columnName;
    string typeName;
    smatch result;
    string s=str;

    //提取关键字
    regex Exp("alter +table +(\\w+) +modify +column +(\\w+) +(\\w+(\\(\\d*\\))?);*");
    regex_match(s,result,Exp);
    tableName=result[1];
    columnName=result[2];
    typeName=result[3];
    if(typeName.compare("int")!=0&&typeName.compare("string")!=0){
        display->append("wrong type,please use int or string");
        qDebug()<<"wrong type,please use int or string";
        return;
    }
    qDebug()<<"table="<<QString::fromStdString(tableName)<<"column"<<QString::fromStdString(columnName)<<"type"<<QString::fromStdString(typeName);

    //向xml文件删除字段
    fie_inf fi;
    fi.modify_XML(filePath,QString::fromStdString(tableName),QString::fromStdString(columnName),QString::fromStdString(typeName),display);
}
