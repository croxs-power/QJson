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

    //
    json_object obj1;
    obj1.insert("name", "obj1");

    //
    json_array array;
    array.append(1);
    array.append(true);
    array.append("string");
    array.append(tr("中文乱码"));
    array.append(12.324);
    array.append(obj1);
    qDebug() << array.format_string();

    //
    obj.insert("obj", obj1);
    obj.insert("array", array);
    //{"string":"hello world","string_zh":"英文乱码","double":12.32321,"int":456,"bool":true,"obj":{"name":"obj1"}}
    qDebug() << obj.format_string();

    //
    qDebug() << obj.value("int").toInt() << obj.value("string_zh").toString();
    json_object obj2 = obj.object("obj");
    if(obj2)
    {
        qDebug() << obj2.value("name").toString();
    }
    json_array array1 = obj.array("array");
    if(array1)
    {
        for(int i = 0; i < array1.size() - 1; i++)
            qDebug() << array1.value(i).toString();
        qDebug() << array1.object(5).format_string();
    }
}
