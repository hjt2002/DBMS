#include "ui_regist.h"
#include "ui_ui_regist.h"

ui_regist::ui_regist(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ui_regist)
{
    ui->setupUi(this);
    setWindowTitle("Registe");
    //连接函数
    connect(ui->pushButton_confirm,SIGNAL(clicked()),this,SLOT(reg_confirm_slot()));
    //回退
    connect(ui->pushButton_back,SIGNAL(clicked()),this,SLOT(reg_back_slot()));
}

ui_regist::~ui_regist()
{
    delete ui;
}

//槽函数
void ui_regist::reg_confirm_slot(){

    account = ui->lineEdit_act->text();
    password = ui->lineEdit_pas->text();
    //检测账号是否已经存在
    //创建迭代器
    bool flag = true;
    QVector<reg_inf*>::iterator iter;
    xml_inf myxml;

    //从文件中读取用户信息
//    QString str = "E:/QTworkspace/data/user.xml";
    QString str = global::getfilepath();
    str.append("/user.xml");

    myxml.read_user_xml(str);
    for(iter = myxml.reg_user_vector.begin();iter != myxml.reg_user_vector.end();iter++){
        if(account == (*iter)->account){
             //账号已经存在
             flag = false;
             break;
        }
    }

    if(flag == false){
        QMessageBox::information(this,"Warning","ACCOUNT HAS EXISTS!",QMessageBox::Warning);
    }else {

        //成功注册
        QMessageBox::information(this,"ok","REGISTE ACCESSED!",QMessageBox::Ok);

        xml_inf myreg;
//      QString str = "E:/QTworkspace/data/user.xml";

        myreg.write_user_xml(str,account,password);

        //用户注册成功，创建用户文件夹
        QString userfile = global::getfilepath();
        userfile.append("/");
        userfile.append(account);

        QDir dir(userfile);
        if(!dir.exists()){
            dir.mkdir(userfile);
            qDebug() << "create user success ! userfile:" + userfile;
        }else{
            qDebug() << "create fail! userfile:" + userfile;
        }
    }

}

void ui_regist::reg_back_slot(){//回退界面

    this->close();
}
