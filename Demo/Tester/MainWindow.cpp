#include "MainWindow.h"

MainWindow::MainWindow(QObject *parent) : QObject(parent)
{

}

void MainWindow::Test()
{
    //
    json_object obj;
    obj.insert("string", "hello world");
    obj.insert("string_zh", tr("中文乱码"));
    obj.insert("double", 12.32321);
    obj.insert("int", 456);
    obj.insert("bool", true);
    //{\"string\":\"hello world\",\"string_zh\":\"中文乱码\",\"double\":12.32321,\"int\":456,\"bool\":true}
    qDebug() << obj.format_string();

    //
    obj.insert("string_zh", tr("英文乱码"));
    //{\"string\":\"hello world\",\"string_zh\":\"英文乱码\",\"double\":12.32321,\"int\":456,\"bool\":true}
    qDebug() << obj.format_string();

    json_object obj1;
    obj1.insert("name", "obj1");
    obj.insert("obj", obj1);
    qDebug() << obj.format_string();
}
