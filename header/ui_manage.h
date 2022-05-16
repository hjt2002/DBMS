#ifndef UI_MANAGE_H
#define UI_MANAGE_H

#include <QWidget>
#include "db_manage.h"
#include "ui_field.h"

namespace Ui {
class ui_manage;
}

class ui_manage : public QWidget
{
    Q_OBJECT

public:
    explicit ui_manage(QWidget *parent = 0);
    ~ui_manage();

private slots:
    void on_pushButton_select_clicked();

    void on_pushButton_create_clicked();

    void on_pushButton_delete_clicked();

    void on_pushButton_back_clicked();

    void on_pushButton_FieldManage_clicked();

private:
    Ui::ui_manage *ui;
    QString dbname;
    database_manage dbman;
    ui_field *ptr;

};

#endif // UI_MANAGE_H
