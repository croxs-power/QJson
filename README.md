# json
基于开源json处理库cJSON，进行二次封装，适用于Qt使用，与Qt的库深度结合。

主要特点：
1、适用于Qt4、Qt5；
2、无需手动释放内存，解决cJSON内存管理的麻烦；
3、使用简单，只有两个类json_object、json_array，本身就支持序列化和反序列化，无需专门的json_document类；
4、使用方便，只需要添加cJSON 和 QJSON总共4个文件；

