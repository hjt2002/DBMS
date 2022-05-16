#-------------------------------------------------
#
# Project created by QtCreator 2022-04-20T19:07:39
#
#-------------------------------------------------

QT       += core gui
QT       += xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DBMS
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    db_table.cpp \
    db_data.cpp \
    ui_login.cpp \
    ui_regist.cpp \
    db_xml.cpp \
    db_manage.cpp \
    ui_manage.cpp \
    field_manage.cpp \
    field_xml.cpp \
    fieldentity.cpp \
    ui_field.cpp \
    path_global.cpp \
    db_powermanage.cpp \
    db_view.cpp

HEADERS += \
        mainwindow.h \
    db_table.h \
    db_data.h \
    ui_login.h \
    ui_regist.h \
    db_xml.h \
    db_manage.h \
    ui_manage.h \
    field_manage.h \
    field_xml.h \
    fieldentity.h \
    ui_field.h \
    path_global.h \
    db_powermanage.h \
    db_view.h


FORMS += \
        mainwindow.ui \
    ui_login.ui \
    ui_regist.ui \
    ui_manage.ui \
    ui_field.ui
