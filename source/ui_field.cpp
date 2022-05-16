#include "ui_field.h"
#include "ui_ui_field.h"


ui_field::ui_field(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ui_field)
{
    ui->setupUi(this);
    setWindowTitle("FIELD");
    fie_inf fi;

    //初始化xml文件
//    QString filePath="C:/Users/L00I/Desktop/DBMS/data/field.xml";
    QString filePath = global::getfilepath().append("/field.xml");
    fi.create_XML(filePath);
}

ui_field::~ui_field()
{
    delete ui;
}

void ui_field::on_close_clicked()
{
    this->close();
}

void ui_field::on_run_clicked()
{
    //字段管理
      ui->text->clear();
      fiename = ui->text->toPlainText();
      fieman.read(fiename,ui->textB);

}
