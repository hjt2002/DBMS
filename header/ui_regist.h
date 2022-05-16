#ifndef UI_REGIST_H
#define UI_REGIST_H

#include <QWidget>
#include "db_xml.h"
#include <qmessagebox.h>
#include "path_global.h"
#include <QDir>

namespace Ui {
class ui_regist;
}

class ui_regist : public QWidget
{
    Q_OBJECT

public:
    explicit ui_regist(QWidget *parent = 0);
    ~ui_regist();

public:

signals:
    void reg_confirm();//确认信号
    void reg_back();//回到主界面信号


public slots:
    void reg_confirm_slot();
    void reg_back_slot();

private:
    Ui::ui_regist *ui;

    //账号、密码
    QString account;
    QString password;

};

#endif // UI_REGIST_H
