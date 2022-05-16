#include "db_data.h"

QString db_data::oname = "";
//正则表达式之前要加\转义才可成功匹配

QString s0="数据项： ";
QString s1="关键字： ";
QString s2="所选表： ";
QString s3="判断条件： ";

using namespace std;

db_data::db_data()
{

}

void db_data::set_dis(QTextBrowser * q)
{
    this->display=q;
//    data_manage.set_dis(q);
}


void db_data::comment(QString s){
    this->display->clear();
    QByteArray ba=s.toLatin1();
    char *str=ba.data();

    isKey(str);

}



void db_data::isKey(char *str)
{


    if(regex_match(str, regex("delete +from +\\w+ *(where +\\w+='?\\w+'?)?;?")))
    {
        this->display->append("<delete comment>");
        data_delete(str);
    }
    else if(regex_match(str, regex("insert +into +\\w+ +values *\\( *'?\\w+'?( *, *'?\\w+'?)* *\\) *;?")))
    {
        this->display->append("<insert comment>");
        data_insert(str);
    }

    else if(regex_match(str, regex("update +\\w+ +set +\\w+='?\\w+'? *(where +\\w+='?\\w+'?)?;?"))){
        this->display->append("<update comment>");
        data_update(str);
    }

    else if(regex_match(str, regex("select *\\* *from +\\w+ *(where +\\w+='?\\w+'?)? *;?")))
    {
        this->display->append("<select* comment>");
        data_select(str);
    }
    else if(regex_match(str, regex("select +\\w+( *, *\\w+)* +from +\\w+ *(where +\\w+='?\\w+'?)? *;?")))
    {
        this->display->append("<select comment>");
        data_select_some(str);
    }
    else if(regex_match(str, regex(" *describe +table +\\w+ *;?")))
    {
        this->display->append("<describe comment>");
        data_describe(str);
    }
    else{this->display->append("语法错误，无法识别！");}

    //日志
    QDateTime current_time = QDateTime::currentDateTime();
    QString currentTime = current_time.toString("yyyy-MM-dd hh:mm:ss");
    QString logstr = QString(str) + " at " + currentTime;
    global::writeToLog(logstr);

}


int db_data::data_describe(char *str){
    string table_name;
    smatch result;
    string s=str;
    regex Exp("( *describe +table +)(\\w+) *;?");//提取关键内容

    regex_match(s,result,Exp);

    table_name=result[2];
    display->append(s2+QString::fromStdString(table_name));


    if(!readFile(table_name,"describe"))
        return 0;

    QString dis_text="";
    for(int i=0;i<num;i++){
        dis_text.append(QString::fromStdString(attributes[i])+" ");
        dis_text.append("("+QString::fromStdString(type[i])+") ");
    }
    display->append(dis_text);

    dis_text="";
    dis_text.append("Contains:   "+QString::number(count));
    display->append(dis_text);

}

int db_data::data_delete(char* str){//删除
    string table_name;
    string judge="";
    smatch result;
    string s=str;
    regex Exp("(delete +from +)(\\w+) *((where +)(\\w+='?\\w+'?))?;?");//提取关键内容

    regex_match(s,result,Exp);

    table_name=result[2];
    display->append(s2+QString::fromStdString(table_name));

    judge=result[5];
    display->append(s3+QString::fromStdString(judge));

    if(!readFile(table_name,"delete"))
        return 0;

    ofstream outfile(table_name+".txt",ios::trunc); //ios::trunc指重新写入
    outfile << "attribute  ";
    outfile <<num<<endl;

    for(int i=0;i<num;i++)
        outfile<<attributes[i]<<"  "<<type[i]<<endl;



    if(judge==""){
        outfile << "content  "<<0<<endl;
        outfile.close();

    }else if(judge!=""){
        int n=0;//删除的个数

        outfile<<endl;

        judge_condition(judge);

        //第一遍统计需要删除的元素个数
        QVector<string> data_string2(data_string);
        QVector<int> data_int2(data_int);
        for(int i=0;i<count;i++){
            bool flag=true;
            for(int j=0;j<num;j++){

                if(judge_type(type[j])==1){
                    a_string=data_string2.front();
                    data_string2.pop_front();
                    if(attributes[j]==column&&a_string==value)
                        flag=false;
                }
                if(judge_type(type[j])==2){
                    a_int=data_int2.front();
                    data_int2.pop_front();
                    if(attributes[j]==column&&a_int==atoi(value.c_str()))
                        flag=false;
                }
            }
            if(!flag)
              n++;
        }

        display->append("删除元素：  "+QString::number(n));

        outfile << "content  "<<count-n<<endl;

        //第二遍进行文件操作，删除符合条件的元素
        for(int i=0;i<count;i++){
            string text="";
            bool flag=true;
            for(int j=0;j<num;j++){

                if(judge_type(type[j])==1){
                    a_string=data_string.front();
                    data_string.pop_front();
                    if(attributes[j]==column&&a_string==value)
                        flag=false;
                    text.append(a_string+"   ");
                }
                if(judge_type(type[j])==2){
                    a_int=data_int.front();
                    data_int.pop_front();
                    if(attributes[j]==column&&a_int==atoi(value.c_str()))
                        flag=false;
                    text.append(to_string(a_int)+"   ");
                }
            }
            if(flag)
              outfile << text<<endl;
        }

    }

    display->append("删除成功");

    return 1;

}


int db_data::data_update(char* str){//更新
    string table_name;
    string judge="";
    string newContent;
    smatch result;
    string s=str;
    regex Exp("(update +)(\\w+)( +set +)(\\w+='?\\w+'?) *((where +)(\\w+='?\\w+'?))?;?");//提取关键内容

    regex_match(s,result,Exp);

    table_name=result[2];
    display->append(s2+QString::fromStdString(table_name));
    QString x="更新内容： ";
    newContent=result[4];
    display->append(x+QString::fromStdString(newContent));


    judge=result[7];
    display->append(s3+QString::fromStdString(judge));

    if(!readFile(table_name,"update"))
        return 0;

    ofstream outfile(table_name+".txt",ios::trunc); //ios::trunc指重新写入
    outfile << "attribute  ";
    outfile <<num<<endl;

    for(int i=0;i<num;i++)
        outfile<<attributes[i]<<"  "<<type[i]<<endl;

    outfile << "content  "<<count<<endl;

    QString sss=QString::fromStdString(newContent);
    QByteArray ba=sss.toLatin1();
    char *p0=ba.data();

    char* p= strtok(p0, " /=/'");
    string column_update=p;
    p= strtok(NULL, " /=/'");
    string value_update=p;

    //cout<<column_update<<value_update<<endl;

    if(judge==""){
        for(int i=0;i<count;i++){
            string text="";

            for(int j=0;j<num;j++){
                if(judge_type(type[j])==1){
                    a_string=data_string.front();
                    data_string.pop_front();
                    if(attributes[j]==column_update)
                        text.append(value_update+"   ");
                    else
                        text.append(a_string+"   ");
                }
                else if(judge_type(type[j])==2){
                    a_int=data_int.front();
                    data_int.pop_front();
                    if(attributes[j]==column_update)
                        text.append(value_update+"   ");
                    else
                        text.append(to_string(a_int)+"   ");
                }


            }
            outfile<<text<<endl;
        }


    }else if(judge!=""){

        judge_condition(judge);

        for(int i=0;i<count;i++){
            bool flag=false;//标记是否修改
            string text="";
            string new_text="";
            for(int j=0;j<num;j++){
                if(judge_type(type[j])==1){
                    a_string=data_string.front();
                    data_string.pop_front();
                    if(attributes[j]==column&&a_string==value)
                        flag=true;

                    //cout<<(attributes[j]==column_update)<<endl;//

                    if(attributes[j]==column_update)
                        new_text.append(value_update+"   ");
                    else
                        new_text.append(a_string+"   ");

                    text.append(a_string+"   ");
                }
                else if(judge_type(type[j])==2){
                    a_int=data_int.front();
                    data_int.pop_front();
                    if(attributes[j]==column&&to_string(a_int)==value)
                        flag=true;

                    if(attributes[j]==column_update)
                        new_text.append(value_update+"   ");
                    else
                        new_text.append(to_string(a_int)+"   ");

                    text.append(to_string(a_int)+"   ");
                }
            }
            //cout<<new_text<<endl;//
            if(flag)
                outfile << new_text<<endl;
            else
                outfile << text<<endl;
        }


    }

    outfile.close();
    display->append("修改成功");
    return 1;
}



int db_data::data_insert(char* str){//插入
    QVector<string> record;
    string table_name;

    string newContent;
    smatch result;
    string s=str;
    regex Exp("(insert +into +)(\\w+)( +values *)(\\( *)('?\\w+'?( *, *'?\\w+'?)*)( *\\) *);?");//提取关键内容

    regex_match(s,result,Exp);

    table_name=result[2];
    display->append(s2+QString::fromStdString(table_name));

    newContent=result[5];
    QString x="插入内容： ";
    display->append(x+QString::fromStdString(newContent));

    newContent=newContent;

    QString sss=QString::fromStdString(newContent);
    QByteArray ba=sss.toLatin1();
    char *p0=ba.data();

    char* p= strtok(p0, " /,/'");
    record.push_back(p);
    p= strtok(NULL, " /,/'");
    while(p!=NULL){
        record.push_back(p);
        p= strtok(NULL, " /,/'");
    }



    if(!readFile(table_name,"insert"))
        return 0;

    ofstream outfile(table_name+".txt",ios::trunc); //ios::app指追加写入
    outfile << "attribute  ";
    outfile <<num<<endl;

    for(int i=0;i<num;i++)
        outfile<<attributes[i]<<"  "<<type[i]<<endl;

    outfile << "content  ";
    count++;
    outfile <<count<<endl;

    for(int i=0;i<count-1;i++){
        for(int j=0;j<num;j++){
            if(judge_type(type[j])==1){
                a_string=data_string.front();
                data_string.pop_front();
                outfile<<a_string<<"  ";
            }
            if(judge_type(type[j])==2){
                a_int=data_int.front();
                data_int.pop_front();
                outfile<<a_int<<"  ";
            }
        }
        outfile<<endl;
    }

    for(int i=0;i<num;i++)
        outfile << record[i]+"  " ;


    outfile.close();

    display->append("插入成功");
    return 1;
}


int db_data::data_select(char* str){ //查询
    string table_name;
    string judge="";
    smatch result;
    string s=str;
    regex Exp("(select *\\* *from +)(\\w+) *((where +)(\\w+='?\\w+'?))? *;?");//提取关键内容

    regex_match(s,result,Exp);

    table_name=result[2];
    display->append(s2+QString::fromStdString(table_name));

    judge=result[5];
    display->append(s3+QString::fromStdString(judge));


    if(!readFile(table_name,"select"))
        return 0;

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


int db_data::data_select_some(char *str){

    string table_name;
    string judge="";
    smatch result;
    string s=str;
    QVector<string> select_col;
    string some_col;
    regex Exp("(select +)(\\w+( *, *\\w+)*)( +from +)(\\w+) *((where +)(\\w+='?\\w+'?))? *;?");//提取关键内容

    regex_match(s,result,Exp);

//    for(int i=0;i<result.size();i++)
//        cout<<result[i]<<endl;

    some_col=result[2];
    table_name=result[5];
    judge=result[8];

    display->append(s2+QString::fromStdString(table_name));
    display->append(s3+QString::fromStdString(judge));

    if(!readFile(table_name,"select"))
        return 0;

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

bool db_data::readFile(string table_name,QString opt){


    //判断是否是访问别的表
    if(db_data::oname.isEmpty()){
        path = (global::getfilepath() + "/" + global::getUsername() +"/" + global::getDBname() + "/").toStdString();
    }else{
        //从power文件中寻找路径
        db_power power;
//        QString temppath = global::getfilepath().append("/" + oname + "/power.txt");
//        power->readPower(oname);//读取power数据到内存中
        power.read(global::getUsername());
        //比较
        QVector<powerStruct*>::iterator iter;
        for(iter = power.vector.begin();iter != power.vector.end();iter++){
            if((*iter)->username == oname && ((*iter)->tablename).toStdString() == table_name
                    &&(*iter)->power == opt){
                path = (global::getfilepath() + "/" + oname +"/" + (*iter)->dbname +"/").toStdString();
                break;
            }
        }
    }

    ifstream infile(path+table_name+".txt");

    if(!infile.is_open()){
        //display->clear();
        display->append("\n error:table不存在 ");
        return false;
    }

    string a; //读取attribute

    infile >> a;
    infile >> num;             // cout << a <<num<<endl;


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

    return true;

}

int db_data::judge_type(string s){
    if(s.compare("string")==0)
        return 1;
    else if(s.compare("int")==0)
        return 2;
}


void db_data::judge_condition(string judge){
    QString sss=QString::fromStdString(judge);
    QByteArray ba=sss.toLatin1();
    char *p0=ba.data();

    char* p= strtok(p0, " /=/'");
    column=p;
    p= strtok(NULL, " /=/'");
    value=p;
}

