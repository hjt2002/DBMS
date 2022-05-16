#include "db_table.h"


db_table::db_table()
{

}

void db_table::set_dis(QTextBrowser* b)
{
    this->display=b;
}


void db_table::comment(QString s)
{
    this->display->clear();
    QByteArray ba=s.toLatin1();
    char *str=ba.data();

    if(regex_match(str,regex(" *drop +table \\w+ *;?")))
    {
        this->display->append("<drop table comment>");
        table_drop(str);

    }else if(regex_match(str,regex(" *create +table +\\w+ *\\( *\\w+ +\\w+ *(, *\\w+ +\\w+ *)*\\) *;?")))
    {
        this->display->append("<create table comment>");
        table_create(str);

    }
    else{display->append("语法错误，无法识别!!!");}
}





int db_table::table_create(char * str){
    QString table_name;
    int num=0;//记录有几个属性
    QVector<QString> attributeName;//存储属性名
    QVector<QString> dataType;//存储数据类型

    display->clear();
    display->append("创建表: \n表名：  ");




    str = strtok(str, " /,/;");
    char *p;

    p = strtok(NULL, " /,/;");//跳过table
    p = strtok(NULL, " /,/;");

    table_name=p;

    display->insertPlainText(table_name);//输出表名
    display->append("属性列及数据类型：");

    p = strtok(NULL, " /(/,/)/;");
    while(p!= NULL){
        num++;
        attributeName.push_back(p);
        p = strtok(NULL, " /(/,/)/;");
        dataType.push_back(p);
        display->append(attributeName[num-1]+"   "+ dataType[num-1]);
        p = strtok(NULL, " /(/,/)/;");
    }

    display->append("共有： "+QString::number(num)+" 个属性");

    path = global::getfilepath().append("/" + global::getUsername() + "/" + global::getDBname() + "/").toStdString();
    ofstream outfile(path+table_name.toStdString()+".txt",ios::trunc); //ios::trunc指重新写入           /* <!!!!!!!!!!!!!!!>*/


    outfile<< "attribute  "<<num<<endl;
    string s;

    for(int i=0;i<num;i++)
        outfile<<attributeName[i].toStdString()<<"  "<<dataType[i].toStdString()<<endl;

    outfile<<"content  "<<0<<endl;


    outfile.close();

    display->append("创建表成功");

    return 1;


}


int db_table::table_drop(char * str){

    string table_name;
    smatch result;
    string s=str;

    regex Exp("( *drop +table )(\\w+) *;?");//提取关键内容

    regex_match(s,result,Exp);

    table_name=result[2];
    display->append("删除表：  "+QString::fromStdString(table_name));

    path = global::getfilepath().append("/" + global::getUsername() + "/" + global::getDBname() + "/").toStdString();

    ifstream infile(path+table_name+".txt");                                      /* <!!!!!!!!!!!!!!!>*/

    if(!infile.is_open()){
        display->append("\n error:table不存在 ");
        return -1;
    }

    infile.close();

    string x=path+table_name+".txt";//表txt路径                                 /* <!!!!!!!!!!!!!!!>*/
    if(remove(x.data())==0)//表删除
        display->append("删除表成功");
    else
        display->append("该表已被打开或正在使用，无法删除");

}

int db_table::judge_type(string s){
    if(s.compare("string")==0)
        return 1;
    else if(s.compare("int")==0)
        return 2;
}
