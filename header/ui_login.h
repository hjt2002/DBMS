#ifndef UI_LOGIN_H
#define UI_LOGIN_H

#include <QWidget>
#include <qmessagebox.h>
#include <ui_regist.h>
#include <QVector>
#include "path_global.h"


namespace Ui {
class ui_login;
}

class ui_login : public QWidget
{
    Q_OBJECT

public:
    explicit ui_login(QWidget *parent = 0);
    ~ui_login();

signals://信号定义
    void login();//登录界面信号
    void close_window();//关闭登录界面信号

public slots://槽函数定义

    void btn_log_clicked( );//登录按钮按下触发的事件


private slots://注册按钮相应槽函数

    void on_pushButton_registe_clicked();

public:

    Ui::ui_login *ui;
    xml_inf myxml;
    ui_regist *m_reg;//定义注册界面类指针

    QString account,password;//从文件中读取的用户信息

};

#endif // UI_LOGIN_H
