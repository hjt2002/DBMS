#include <db_xml.h>

void xml_inf::read_user_xml(QString user_dir){

    QString nodename;
    //QString output;
    QFile file(user_dir);

    if(!file.open(QFile::ReadOnly|QFile::Text)){
        qDebug() << "can not open file!";

    }else{

        qDebug() << "file open!" << "dir: " << user_dir;
    }

    QXmlStreamReader xmlreader(&file);
    //qDebug() << "xml文件存在且成功打开！";

    while(!xmlreader.atEnd() || !xmlreader.hasError()){//正确读取

        //创建注册信息结构体指针
        reg_inf *reg = new reg_inf();
        //读取非根节点的第一个子节点
        xmlreader.readNextStartElement();

        //第一个子节点
        nodename = xmlreader.name().toString();
        //nodename = xmlreader.name();

        if(nodename == "user" && xmlreader.isStartElement()){

            //读取附加信息
            //xmlreader.attributes().value("id").toString();
            //qDebug() << "id" << xmlreader.attributes().value("id").toString();
            reg->id = xmlreader.attributes().value("id").toString();//对结构体赋值
            //output.append(QString("id:%1 ").arg(xmlreader.attributes().value("id").toString()));

            while(!(nodename == "user" && xmlreader.isEndElement())){
                //子节点内容没结束
                xmlreader.readNextStartElement();
                nodename = xmlreader.name().toString();

                if(nodename == "account" && xmlreader.isStartElement() ){
                    while(!(nodename == "account" && xmlreader.isEndElement())){
                        //output.append(QString("accout:%1 ").arg(xmlreader.readElementText()));
                        //qDebug() << "accout: "<< xmlreader.readElementText();

                        //对account赋值
                        (*reg).account = xmlreader.readElementText();
                    }

                }else if(nodename == "password" && xmlreader.isStartElement()){

                    while(!(nodename == "password" && xmlreader.isEndElement())){
                        //output.append(QString("password:%1 ").arg(xmlreader.readElementText()));
                        //qDebug() << "password: "<< xmlreader.readElementText();

                        //对password赋值
                        (*reg).password = xmlreader.readElementText();

                    }
                }
            }
        }
        reg_user_vector.push_back(reg);
    }
        //qDebug() << "xml:\n" << output;
        file.close();
}



void xml_inf::write_user_xml(QString user_dir,QString account,QString password){

    int userNum = 0;
    QFile file(user_dir);
    if(!file.open(QFile::ReadOnly)){
        qDebug() << "fail to open file!";
    }else{
        qDebug() << "file open!" << "dir:" <<user_dir;
    }

    QDomDocument doc;
    if(!doc.setContent(&file)){
            qDebug() << "doc set!";
    }

    file.close();
    //从文件中读取用户信息
//    QString str = "E:/QTworkspace/data/user.xml";
    QString str = global::getfilepath();
    str.append("/user.xml");
    xml_inf myxml;
    myxml.read_user_xml(str);
    QVector<reg_inf*>::iterator iter;
    for(iter = myxml.reg_user_vector.begin();iter != myxml.reg_user_vector.end();iter++){
        if(account == (*iter)->account){
             userNum +=1;
        }
    }
    QString str1 = QString::number(userNum);


    //根节点
    QDomElement root = doc.documentElement();
    //创建一级子节点
    QDomElement user = doc.createElement("user");
    user.setAttribute("id",str1);

    //创建二级子节点
    QDomElement accNode = doc.createElement("account");
    QDomText text1 = doc.createTextNode(account);
    accNode.appendChild(text1);
    QDomElement pasNode = doc.createElement("password");
    QDomText text2 = doc.createTextNode(password);
    pasNode.appendChild(text2);

    user.appendChild(accNode);
    user.appendChild(pasNode);

    root.appendChild(user);

    if(!file.open(QFile::WriteOnly|QFile::Truncate)){
        qDebug() << "open failed to truncate";
    }

    QTextStream output_stream(&file);
    doc.save(output_stream,4);
    file.close();
}
