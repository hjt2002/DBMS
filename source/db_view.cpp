#include "db_view.h"

using namespace std;



db_view::db_view()
{

}



void db_view::set_dis(QTextBrowser * q)
{
    this->display=q;
}

void db_view::comment(QString s){
    this->display->clear();
    QByteArray ba=s.toLatin1();
    char *str=ba.data();
    display->append("<view comment>");

    if(regex_match(str,regex(" *drop +view +\\w+ *;?")))
    {
        this->display->append("<drop view comment>");
        view_drop(str);

    }else if(regex_match(str,regex(" *create +view +\\w+ +as +select *\\* *from +\\w+ *;?"))){
        this->display->append("<create view comment>");
        view_create(str);
    }else if(regex_match(str,regex(" *select *\\* *from +view +\\w+ *(where +\\w+='?\\w+'?)? *;?"))){
        this->display->append("<select* view comment>");
        view_select(str);
    }else if(regex_match(str,regex(" *select +\\w+( *, *\\w+)* +from +view +\\w+ *(where +\\w+='?\\w+'?)? *;?"))){
        this->display->append("<select some view comment>");
        view_select_some(str);
    }

    else{this->display->append("语法错误，无法识别!！");}


}

int db_view::view_select(char * str){


    string view_name;
    string judge="";
    smatch result;
    string s=str;

    regex Exp("( *select *\\* *from +view +)(\\w+) *((where +)(\\w+='?\\w+'?))? *;?");//提取关键内容

    regex_match(s,result,Exp);

    view_name=result[2];
    display->append(s2+QString::fromStdString(view_name));

    judge=result[5];
    display->append(s3+QString::fromStdString(judge));


    if(!read_view_File(view_name))
        return 0;

    display->append("该视图下的基本表：  "+QString::fromStdString(view_table_name));

    view_path = global::getfilepath().append("/" + global::getUsername() +"/" + global::getDBname() + "/").toStdString();
    table_path = global::getfilepath().append("/" + global::getUsername()+"/" + global::getDBname() + "/").toStdString();

    ifstream infile(table_path+view_table_name+".txt");                                 /* <!!!!!!!!!!!!!!!>*/
    if(!infile.is_open()){
        display->append("\n error:该视图下的基本表已不存在！ ");
        return false;
    }

    string a; //读取attribute

    infile >> a;
    infile >> num;
    string attributes_info;
    for(int i=0;i<num;i++){
        infile >> attributes_info;
        attributes.push_back(attributes_info);
        infile >> attributes_info;
        type.push_back(attributes_info);                  // cout << attributes[i] <<type[i]<<endl;
    }
    string c;
    infile>>c;//读取content
    infile>>count;//读取记录数

    for(int i=0;i<count;i++)
        for(int j=0;j<num;j++){
            if(judge_type(type[j])==1){
                infile>>a_string;
                data_string.push_back(a_string);
            }
            if(judge_type(type[j])==2){
                infile>>a_int;
                data_int.push_back(a_int);
            }

        }

    infile.close();


    QString dis_text="";
    for(int i=0;i<num;i++){
        dis_text.append(QString::fromStdString(attributes[i])+" ");
        dis_text.append("("+QString::fromStdString(type[i])+") ");
    }
    dis_text.append("\n");
    display->append(dis_text);

    if(judge==""){

        for(int i=0;i<count;i++){
            dis_text="";
            for(int j=0;j<num;j++){
                if(judge_type(type[j])==1){
                    a_string=data_string.front();
                    data_string.pop_front();
                    dis_text.append(QString::fromStdString(a_string)+"   ");
                }
                if(judge_type(type[j])==2){
                    a_int=data_int.front();
                    data_int.pop_front();
                    dis_text.append(QString::number(a_int)+"   ");
                }
            }
            display->append(dis_text);
        }

    }
    else if(judge!=""){

        judge_condition(judge);

        for(int i=0;i<count;i++){
            dis_text="";
            bool flag=true;
            for(int j=0;j<num;j++){

                if(judge_type(type[j])==1){
                    a_string=data_string.front();
                    data_string.pop_front();
                    if(attributes[j]==column&&a_string!=value)
                        flag=false;
                    dis_text.append(QString::fromStdString(a_string)+"   ");
                }
                if(judge_type(type[j])==2){
                    a_int=data_int.front();
                    data_int.pop_front();
                    if(attributes[j]==column&&a_int!=atoi(value.c_str()))
                        flag=false;
                    dis_text.append(QString::number(a_int)+"   ");
                }
            }
            if(flag)
              display->append(dis_text);
        }


    }



    return 1;
}


int db_view::view_select_some(char * str){


    string view_name;
    string judge="";
    smatch result;
    string s=str;
    QVector<string> select_col;
    string some_col;
    regex Exp("( *select +)(\\w+( *, *\\w+)*)( +from +view +)(\\w+) *((where +)(\\w+='?\\w+'?))? *;?");//提取关键内容

    regex_match(s,result,Exp);

//    for(int i=0;i<result.size();i++)
//        cout<<result[i]<<endl;

    some_col=result[2];
    view_name=result[5];
    judge=result[8];

    display->append(s2+QString::fromStdString(view_name));
    display->append(s3+QString::fromStdString(judge));

    if(!read_view_File(view_name))
        return 0;

    display->append("该视图下的基本表：  "+QString::fromStdString(view_table_name));
    view_path = global::getfilepath().append("/" + global::getUsername() +"/" + global::getDBname() + "/").toStdString();
    table_path = global::getfilepath().append("/" + global::getUsername()+"/" + global::getDBname() + "/").toStdString();

    ifstream infile(table_path+view_table_name+".txt");                                /* <!!!!!!!!!!!!!!!>*/
    if(!infile.is_open()){
        display->append("\n error:该视图下的基本表已不存在！ ");
        return false;
    }


    string a; //读取attribute

    infile >> a;
    infile >> num;
    string attributes_info;
    for(int i=0;i<num;i++){
        infile >> attributes_info;
        attributes.push_back(attributes_info);
        infile >> attributes_info;
        type.push_back(attributes_info);
    }
    string c;
    infile>>c;//读取content
    infile>>count;//读取记录数

    for(int i=0;i<count;i++)
        for(int j=0;j<num;j++){
            if(judge_type(type[j])==1){
                infile>>a_string;
                data_string.push_back(a_string);
            }
            if(judge_type(type[j])==2){
                infile>>a_int;
                data_int.push_back(a_int);
            }

        }

    infile.close();




    QString sss=QString::fromStdString(some_col);
    QByteArray ba=sss.toLatin1();
    char *p0=ba.data();

    char* p= strtok(p0, " /,/'");
    while(p!=NULL){
        select_col.push_back(p);
        p= strtok(NULL, " /,/'");
    }

    QString dis_text="";
    for(int i=0;i<num;i++){
        for(int j=0;j<select_col.size();j++){
            if(select_col[j]==attributes[i]){
                dis_text.append(QString::fromStdString(attributes[i])+" ");
                dis_text.append("("+QString::fromStdString(type[i])+") ");
            }
        }

    }
    dis_text.append("\n");
    display->append(dis_text);

    if(judge==""){

        for(int i=0;i<count;i++){
            dis_text="";
            for(int j=0;j<num;j++){
                if(judge_type(type[j])==1){
                    a_string=data_string.front();
                    data_string.pop_front();
                    for(int k=0;k<select_col.size();k++){
                        if(select_col[k]==attributes[j])
                               dis_text.append(QString::fromStdString(a_string)+"   ");
                    }
                }
                if(judge_type(type[j])==2){
                    a_int=data_int.front();
                    data_int.pop_front();
                    for(int k=0;k<select_col.size();k++){
                        if(select_col[k]==attributes[j])
                               dis_text.append(QString::number(a_int)+"   ");;
                    }
                }
            }
            display->append(dis_text);
        }

    }else if(judge!=""){

        judge_condition(judge);

        for(int i=0;i<count;i++){
            dis_text="";
            bool flag=true;
            for(int j=0;j<num;j++){


                if(judge_type(type[j])==1){
                    a_string=data_string.front();
                    data_string.pop_front();
                    if(attributes[j]==column&&a_string!=value)
                        flag=false;
                    for(int k=0;k<select_col.size();k++){
                        if(select_col[k]==attributes[j])
                               dis_text.append(QString::fromStdString(a_string)+"   ");
                    }
                }
                if(judge_type(type[j])==2){
                    a_int=data_int.front();
                    data_int.pop_front();
                    if(attributes[j]==column&&a_int!=atoi(value.c_str()))
                        flag=false;
                    for(int k=0;k<select_col.size();k++){
                        if(select_col[k]==attributes[j])
                               dis_text.append(QString::number(a_int)+"   ");;
                    }
                }
            }
            if(flag)
              display->append(dis_text);
        }


    }

}



int db_view::view_create(char *str){
    string view_name;
    smatch result;
    string s=str;
    regex Exp("( *create +view +)(\\w+)( +as +select *\\* *from +)(\\w+) *;?");//提取关键内容

    regex_match(s,result,Exp);

    view_name=result[2];
    view_table_name=result[4];
    display->append("创建视图：  "+QString::fromStdString(view_name));
    display->append("基于表：  "+QString::fromStdString(view_table_name));

    view_path = global::getfilepath().append("/" + global::getUsername() +"/" + global::getDBname() + "/").toStdString();
    table_path = global::getfilepath().append("/" + global::getUsername()+"/" + global::getDBname() + "/").toStdString();

    ifstream infile(table_path+view_table_name+".txt");                                /* <!!!!!!!!!!!!!!!>*/

    if(!infile.is_open()){
        display->append("\n error:table不存在 ");
        return -1;
    }

    infile.close();

    ofstream outfile(view_path+view_name+".txt",ios::trunc); //ios::trunc指重新写入           /* <!!!!!!!!!!!!!!!>*/


    outfile<< s<<"\n";//写入完整的创建语句
    outfile<<view_table_name<<endl;//写入基本表名字


    outfile.close();

    display->append("创建视图成功");

    return 1;


}

int db_view::view_drop(char * str){

    string view_name;
    smatch result;
    string s=str;
    regex Exp("( *drop +view +)(\\w+) *;?");//提取关键内容

    regex_match(s,result,Exp);

    view_name=result[2];
    display->append(s2+QString::fromStdString(view_name));


    if(!read_view_File(view_name))
        return 0;

    view_path = global::getfilepath().append("/" + global::getUsername() +"/" + global::getDBname() + "/").toStdString();
    table_path = global::getfilepath().append("/" + global::getUsername()+"/" + global::getDBname() + "/").toStdString();

    string x=view_path+view_name+".txt";//视图txt路径                                 /* <!!!!!!!!!!!!!!!>*/
    remove(x.data());//视图删除

    display->append("删除视图成功");
    return 1;
}



bool db_view::read_view_File(string view_name){

    view_path = global::getfilepath().append("/" + global::getUsername() +"/" + global::getDBname() + "/").toStdString();
    table_path = global::getfilepath().append("/" + global::getUsername()+"/" + global::getDBname() + "/").toStdString();

    ifstream infile(view_path+view_name+".txt");                                /* <!!!!!!!!!!!!!!!>*/

    if(!infile.is_open()){
        display->append("\n error:view不存在 ");
        return false;
    }

    string s;
    getline(infile,s);
    infile>>view_table_name;


    infile.close();

    return true;

}

int db_view::judge_type(string s){
    if(s.compare("string")==0)
        return 1;
    else if(s.compare("int")==0)
        return 2;
}


void db_view::judge_condition(string judge){
    QString sss=QString::fromStdString(judge);
    QByteArray ba=sss.toLatin1();
    char *p0=ba.data();

    char* p= strtok(p0, " /=/'");
    column=p;
    p= strtok(NULL, " /=/'");
    value=p;
}
