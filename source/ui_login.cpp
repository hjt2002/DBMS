#include "ui_login.h"
#include "ui_ui_login.h"
#include "db_xml.h"

ui_login::ui_login(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ui_login)
{
    ui->setupUi(this);
    //注册界面
    m_reg = new ui_regist;

    setWindowTitle("Login");
    //connect 信号槽

    //触发登录按钮的信号槽的连接
    connect(ui->pushButton_login,SIGNAL(clicked()),this,SLOT(btn_log_clicked()));
    //发出信号后关闭登录窗口的信号槽连接
    connect(this,SIGNAL(close_window()),this,SLOT(close()));

}

//登录按钮触发事件
void ui_login::btn_log_clicked()
{
    //从文件中读取用户信息
//  QString str = "E:/QTworkspace/data/user.xml";
    QString str = global::getfilepath();
    str = str.append("/user.xml");
    myxml.read_user_xml(str);


    //获取账户密码
    QString in_account = ui->lineEdit_account->text();
    QString in_password = ui->lineEdit_password->text();


    account = in_account;
    password = in_password;

    //创建迭代器
    bool flag = false;
    QVector<reg_inf*>::iterator iter;
    for(iter = myxml.reg_user_vector.begin();iter != myxml.reg_user_vector.end();iter++){
        if(account == (*iter)->account && !account.isEmpty()){
            if(password == (*iter)->password && !password.isEmpty()){
                //匹配成功
                flag = true;

                global::setUsername(account);//确定当前用户

                emit(login());
                emit(close_window());

            }else{
                //密码错误
                QMessageBox::information(this,"Warning","PASSWORD WRONG");
            }
        }
    }

    if(flag == false){
        QMessageBox::information(this,"Warning","ACCOUNT WRONG!");
    }



//    //账户密码匹配
//    if(account == admin_account && password == admin_password){
//        //发出登录信号
//        emit(login());
//        //发出关闭窗口的信号
//        emit(close_window());

//    }else if(account != admin_account || account == NULL){//账户空或错误
//        QMessageBox::information(this,"Warning","ACCOUNT WRONG!");
//    }else if(password != admin_account || account == NULL){//密码空或错误
//        QMessageBox::information(this,"Warning","PASSWORD WRONG");
//    }

}


ui_login::~ui_login()
{
    delete ui;
}

void ui_login::on_pushButton_registe_clicked()//注册的槽函数
{
    m_reg->show();//显示注册界面
}






