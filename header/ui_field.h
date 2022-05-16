#ifndef UI_FIELD_H
#define UI_FIELD_H

#include <QWidget>
#include <fieldentity.h>
#include <field_xml.h>
#include <field_manage.h>
#include "path_global.h"

namespace Ui {
class ui_field;
}

class ui_field : public QWidget
{
    Q_OBJECT

public:
    explicit ui_field(QWidget *parent = 0);
    ~ui_field();

private slots:
    void on_close_clicked();

    void on_run_clicked();

private:
    Ui::ui_field *ui;
    QString fiename;
    field_manage fieman;
};

#endif // UI_FIELD_H
