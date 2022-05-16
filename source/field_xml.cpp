#include "field_xml.h"

void fie_inf::create_XML(QString filePath){
    QFile file(filePath);
    if(true==file.exists()){
        qDebug()<<"文件存在";
        return;
    }else{
        bool isOk=file.open(QIODevice::WriteOnly);

        if(true==isOk){
            //初始化根节点
            QDomDocument doc;
            //加入说明
            QDomProcessingInstruction ins;
            ins=doc.createProcessingInstruction("xml","version=\'1.0\' encoding='utf-8'");
            doc.appendChild(ins);
            //根节点名
            QDomElement root=doc.createElement("field");
            doc.appendChild(root);
            //写入xml文件
            QTextStream stream(&file);
            doc.save(stream,4);
            file.close();
        }else{
            qDebug()<<"WriteOnly error";
            return;
        }
    }
}

void fie_inf::add_XML(QString filePath,QString table,QString column,QString type,QTextBrowser *display){
    QFile file(filePath);
    //打开文件
    if(!file.open(QFile::ReadOnly)){
        display->append("fail to open file!");
        qDebug() << "fail to open file!";
    }else{
        qDebug() << "file open!" << "dir:" <<filePath;
    }

    QDomDocument doc;
    if(!doc.setContent(&file)){
            qDebug() << "doc set!";
    }

    file.close();

    //根节点
    QDomElement root=doc.documentElement();

    bool sametable=false;
    bool samecolumn=false;
    int samet;

    //创建一级子节点,寻找相同表名的节点
    QDomNodeList list=doc.elementsByTagName("表名");
    for(int i=0;i<list.length();i++){
        if(list.at(i).attributes().namedItem("table").nodeValue()==table){
            display->append("find same table");
            qDebug()<<"find same table";
            sametable=true;
            samet=i;
        }
    }
    //寻找相同字段名
    if(sametable){
        QDomNodeList list2=list.at(samet).childNodes();
        for(int i=0;i<list2.length();i++){
            if(list2.at(i).attributes().namedItem("column").nodeValue()==column){
                samecolumn=true;
            }
        }
    }else{
        display->append("can't find same table");
        qDebug()<<"can't find same table";
    }
    //发现相同字段名则退出
    if(true==samecolumn){
        display->append("find same column");
        qDebug()<<"find same column";
        display->append("add error");
        return;
    }
    QDomElement field=doc.createElement("表名");
    field.setAttribute("table",table);

    //创建二级子节点
    QDomElement Node1=doc.createElement("字段名");
    Node1.setAttribute("column",column);//加入属性
    //创建三级子节点
    QDomElement Node2=doc.createElement("type");//字段类型
    QDomText text2=doc.createTextNode(type);//加入文本
    QDateTime date=QDateTime::currentDateTime();//时间
    QString dateStr=date.toString("yyyy-MM-dd hh:mm:ss");
    QDomElement dateEmt=doc.createElement("日期");
    QDomAttr dateAttr=doc.createAttribute("date");//加入属性
    dateAttr.setNodeValue(dateStr);
    dateEmt.setAttributeNode(dateAttr);

    Node2.appendChild(text2);
    Node1.appendChild(dateEmt);
    Node1.appendChild(Node2);

    //加入相同的表内或新建的表
    if(true==sametable){
        list.at(samet).appendChild(Node1);
    }else{
        field.appendChild(Node1);
        root.appendChild(field);
    }

    if(!file.open(QFile::WriteOnly|QFile::Truncate)){
        qDebug()<< "open failed to truncate";
    }
    //写入文件
    QTextStream output_stream(&file);
    doc.save(output_stream,4);
    file.close();

    display->append("add successs");
    qDebug()<<"add successs";
}

void fie_inf::modify_XML(QString filePath,QString table,QString column,QString type,QTextBrowser *display){
    QFile file(filePath);
    //打开文件
    if(!file.open(QFile::ReadOnly)){
        display->append("fail to open file!");
        qDebug() << "fail to open file!";
    }else{
        qDebug() << "file open!" << "dir:" <<filePath;
    }

    QDomDocument doc;
    if(!doc.setContent(&file)){
            qDebug() << "doc set!";
    }

    file.close();

    bool sametable=false;
    bool samecolumn=false;
    int samet;

    //创建一级子节点,寻找相同表名的节点
    QDomNodeList list=doc.elementsByTagName("表名");
    for(int i=0;i<list.length();i++){
        if(list.at(i).attributes().namedItem("table").nodeValue()==table){
            sametable=true;
            samet=i;
        }
    }
    //寻找相同字段的节点
    if(sametable){
        QDomNodeList list2=list.at(samet).childNodes();
        for(int i=0;i<list2.length();i++){
            if(list2.at(i).attributes().namedItem("column").nodeValue()==column){
                samecolumn=true;
                display->append("find same column");
                qDebug()<<"find same column";
                QDateTime date=QDateTime::currentDateTime();
                QString dateStr=date.toString("yyyy-MM-dd hh:mm:ss");
                list2.at(i).childNodes().at(0).setNodeValue(dateStr);//修改时间
                list2.at(i).childNodes().at(1).childNodes().at(0).setNodeValue(type);//修改字段类型
                break;
            }
        }
    }else{
        display->append("can't find same table");
        qDebug()<<"can't find same table";
    }

    //发现无目标字段名则退出
    if(false==samecolumn){
        display->append("can't find same column");
        qDebug()<<"can't find same column";
        display->append("modify error");
        return;
    }
    if(!file.open(QFile::WriteOnly|QFile::Truncate)){
        qDebug()<< "open failed to truncate";
    }
    //写入文件
    QTextStream output_stream(&file);
    doc.save(output_stream,4);
    file.close();

    display->append("modify successs");
    qDebug()<<"modify successs";
}

void fie_inf::drop_XML(QString filePath,QString table,QString column,QTextBrowser *display){
    QFile file(filePath);
    //打开文件
    if(!file.open(QFile::ReadOnly)){
        display->append("fail to open file!");
        qDebug() << "fail to open file!";
    }else{
        qDebug() << "file open!" << "dir:" <<filePath;
    }

    QDomDocument doc;
    if(!doc.setContent(&file)){
            qDebug() << "doc set!";
    }

    file.close();

    bool sametable=false;
    bool samecolumn=false;
    int samet;

    //创建一级子节点,寻找相同表名的节点
    QDomNodeList list=doc.elementsByTagName("表名");
    for(int i=0;i<list.length();i++){
        if(list.at(i).attributes().namedItem("table").nodeValue()==table){
            display->append("find same table");
            qDebug()<<"find same table";
            sametable=true;
            samet=i;
        }
    }
    //寻找相同字段的节点
    if(sametable){
        QDomNodeList list2=list.at(samet).childNodes();
        for(int i=0;i<list2.length();i++){
            if(list2.at(i).attributes().namedItem("column").nodeValue()==column){
                display->append("find same column");
                qDebug()<<"find same column";
                list2.at(i).parentNode().removeChild(list2.at(i));
                break;
            }
        }
    }else{
        display->append("can't find same table");
        qDebug()<<"can't find same table";
    }
    //未找到目标字段则退出
    if(false==samecolumn){
        display->append("can't find same column");
        qDebug()<<"can't find same column";
        display->append("drop error");
        return;
    }


    if(!file.open(QFile::WriteOnly|QFile::Truncate)){
        qDebug()<< "open failed to truncate";
    }
    //写入文件
    QTextStream output_stream(&file);
    doc.save(output_stream,4);
    file.close();

    display->append("drop successs");
    qDebug()<<"drop successs";
}
