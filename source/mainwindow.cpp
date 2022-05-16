#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //通过指针创建登录界面类的对象
    m_log = new ui_login;
    man = new ui_manage;//创建管理界面的指针
    //数据管理初始化
    data_manage.set_dis(ui->display);
    view_manage.set_dis(ui->display);
    table_manage.set_dis(ui->display);
    //调用登录窗口的show函数显示登录界面
    m_log->show();

    //建立信号槽,当接收到登录界面发来的login信号后，调用主窗口的show函数
    connect(m_log,SIGNAL(login()),this,SLOT(show()));
    connect(m_log,SIGNAL(login()),this,SLOT(initTreeWidget()));
    ui->textEdit->setText("please connect database!");
}

MainWindow::~MainWindow()
{
    delete ui;
}
//初始化树形窗体
void MainWindow::initTreeWidget(){

    QString rootpath = global::getfilepath();
    rootpath.append("/" + global::getUsername());

    QTreeWidgetItem *root = new QTreeWidgetItem(ui->treeWidget);
    root->setText(0,global::getUsername());
    QFileInfoList lists = allfile(root,rootpath);
}

void MainWindow::on_pushButton_manage_clicked()
{
    man->show();//显示管理界面
}
void MainWindow::on_pushButton_run_clicked()
{
    //获取输出
    QString str = ui->textEdit->toPlainText();



    if(str.contains("database",Qt::CaseSensitive)){
        //数据库操作create database|drop database
        judgeDatabaseOptr(str);
        return;
    }

    if(dbstate){
        //数据库呈连接状态
        //权限管理
        if(str.contains(".")){
            QString pattern("select.*\\s(\\w+\\.).*");
            QRegExp rx(pattern);

            int pos = str.indexOf(rx);
            if(pos >= 0){
                //目标用户
                QString oname = rx.cap(1);
                oname = oname.left(oname.size()-1);
                data_manage.oname = oname;
                str.replace(rx.cap(1)," ");
            }
        }
        if((str.contains("select")&&str.contains("from"))||str.contains("update")||str.contains("insert")||str.contains("delete from")){
            //数据操作
            if(str.contains("view")){
                view_manage.comment(str);
            }else {
                data_manage.comment(str);
            }

        }else if(str.contains("grant")||str.contains("revoke")){
            //权限管理
            db_power power;
            if(str.contains("grant")){
                power.grantPower(str);
            }else{
                power.revokePower(str);
            }
        }else if(str.contains("create table")||str.contains("drop table")){
            //表管理
            table_manage.comment(str);
        }


    }else{

        ui->display->setText("database don't connected!");

    }
}

void MainWindow::judgeDatabaseOptr(QString str){
    database_manage dm;
    QStringList list = str.split(" ");

    QString dbname = list[2];
    if(dbname.contains(";")){
        dbname.remove(dbname.length()-1,1);//删除行尾分号
    }

    if(str.contains("database",Qt::CaseSensitive)){

        if(str.contains("create",Qt::CaseSensitive)){
            //创建数据库
            dm.createDatabase(dbname);
            ui->display->setText("create database:" + dbname);
        }else if(str.contains("drop",Qt::CaseInsensitive)){
            //删除数据库
            dm.deleteDatabase(dbname,this);
            ui->display->setText("delete database:" + dbname);
        }else if(str.contains("connect",Qt::CaseSensitive)){





            //连接数据库
            global::setDBname(dbname);//设置当前数据库
            //数据库连接成功
            QString tempstr = "current database:" + global::getDBname();
            ui->display->setText(tempstr + "\nconnect successfully!");

            //日志
            QDateTime current_time = QDateTime::currentDateTime();
            QString currentTime = current_time.toString("yyyy-MM-dd hh:mm:ss");
            QString logstr = "database: " + dbname +" connect at " + currentTime;
            global::writeToLog(logstr);
            //设置数据库状态
            dbstate = true;

        }
    }
}

QFileInfoList MainWindow::allfile(QTreeWidgetItem *root, QString path){


    QDir dir(path);//遍历各级子目录
    QDir dir_file(path);//遍历子目录的文件

    //遍历文件夹 添加到widget
    QFileInfoList file_list = dir.entryInfoList(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    QFileInfoList folder_list = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);//获取当前所有目录

    //递归添加各级目录到上一级目录
    for(int i =0;i != folder_list.size();i++){

        QString  namepath = folder_list.at(i).absoluteFilePath();//路径
        QFileInfo folderinfo = folder_list.at(i);
        QString name = folderinfo.fileName();//获取目录名称
        QTreeWidgetItem *childroot = new QTreeWidgetItem(QStringList()<<name);
        root -> addChild(childroot);
        childroot->setText(0,name);
        root->addChild(childroot); //添加为子项
        QFileInfoList child_file_list = allfile(childroot,namepath);//递归
        file_list.append(child_file_list);
        file_list.append(name);

    }

    /*添加path路径文件*/

       dir_file.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);        //获取当前所有文件
       dir_file.setSorting(QDir::Name);//
       QFileInfoList list_file = dir_file.entryInfoList();
       for (int i = 0; i < list_file.size(); ++i) {       //将当前目录中所有文件添加到treewidget中
           QFileInfo fileInfo = list_file.at(i);
           QString name2=fileInfo.fileName();
           QTreeWidgetItem* child = new QTreeWidgetItem(QStringList()<<name2);
           //child->setCheckState(1, Qt::Checked);
           child->setText(0,name2);
           root->addChild(child);  //allfile传入的root下面加入

       }
       return file_list;

}


