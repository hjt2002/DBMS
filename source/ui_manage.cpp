#include "ui_manage.h"
#include "ui_ui_manage.h"

ui_manage::ui_manage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ui_manage)
{
    ui->setupUi(this);
    setWindowTitle("MANAGE");
    ptr = new ui_field;
}

ui_manage::~ui_manage()
{
    delete ui;
}



void ui_manage::on_pushButton_select_clicked()
{
    QString str = global::getfilepath();
    //打开当前工作文件夹
    QDesktopServices::openUrl(QUrl(str,QUrl::TolerantMode));
}

void ui_manage::on_pushButton_create_clicked()
{
    //获取输入的dbname
    dbname = ui->lineEdit->text();
    dbman.createDatabase(dbname);
}

void ui_manage::on_pushButton_delete_clicked()
{
    //获取输入的dbname
    dbname = ui->lineEdit->text();
    dbman.deleteDatabase(dbname,this);
}

void ui_manage::on_pushButton_back_clicked()
{
    this->close();//关闭当前窗口
}

void ui_manage::on_pushButton_FieldManage_clicked()
{

    ptr->show();
}
