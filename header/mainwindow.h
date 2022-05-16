#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <ui_login.h>
#include <ui_manage.h>
#include <db_data.h>
#include "db_table.h"
#include "path_global.h"

#include <QTreeWidgetItem>
#include "QFileInfoList"
#include "qtranslator.h"
#include "QDateTime"
#include "db_powermanage.h"
#include "db_view.h"

#include <QRegExp>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_manage_clicked();

    void on_pushButton_run_clicked();


public slots:
    void initTreeWidget();


private:
    Ui::MainWindow *ui;
    //登录界面类的对象作为指针
    ui_login *m_log;
    ui_manage *man;

    db_data data_manage;
    db_view view_manage;
    db_table table_manage;

    QFileInfoList allfile(QTreeWidgetItem *root,QString path);

    bool dbstate = false;

public:
    void judgeDatabaseOptr(QString str);//判断数据库操作（添加、删除、连接）



};

#endif // MAINWINDOW_H
