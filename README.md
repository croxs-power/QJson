# QJson
基于“开源json处理库cJSON”，采用Qt进行二次封装，与QByteArray、QVariant类相结合，比Qt5自带的Qt处理库方便简单。

# 主要特点：
* 适用于Qt4、Qt5；
* 无需手动释放内存，解决cJSON内存管理的麻烦；
* 使用简单，只有两个类json_object、json_array，本身就支持序列化和反序列化，无需专门的json_document类；
* 使用方便，只需要添加cJSON 和 QJSON总共4个文件；

# 说明
* 对于json中的某些功能，由于暂时没有用到，可能暂未支持，使用者可自行添加

# 示例：
头文件
````C++
class json_interface
{
protected:
    typedef void (*unspecified_bool_type)(json_interface***);

public:
    json_interface();
    json_interface(cJSON *json);
    json_interface(QByteArray json);
    json_interface(const json_interface* obj);
    json_interface(const json_interface& obj);
    virtual ~json_interface();

    // Safe bool conversion operator
    operator bool() const;
    bool operator !() const;
    json_interface& operator=(const json_interface& json);
    bool operator==(const json_interface& json) const;

    bool parse(QByteArray json_string);
    bool is_empty() const;
    QByteArray json_string() const;
    QString format_string() const;
    void clear();

    cJSON *internal_object() const;

protected:
    enum json_type_enum
    {
        json_type_object,
        json_type_array
    };
    void Create(json_type_enum type);

    cJSON *m_json;
    QString m_errorMsg;
};

class json_object: public json_interface
{
public:
    json_object();
    json_object(cJSON *json);
    json_object(QByteArray json);
    json_object(const json_object* obj);
    json_object(const json_object& obj);

    //if key no exist add, or replace it
    bool insert(const QString &key, const QVariant &value);
    bool insert(const QString &key, const json_object &value);
    bool insert(const QString &key, const json_array &value);

    QVariant value(const QString &key);
    json_object object(const QString &key);
    json_array array(const QString &key);

    QStringList keys();
    bool exist(QString key);

    //remove key
    void remove(const QString &key);
};

class json_array: public json_interface
{
public:
    json_array();
    json_array(cJSON *json);
    json_array(QByteArray json);
    json_array(const json_array* obj);
    json_array(const json_array& obj);

    bool append(const QVariant &value);
    bool append(const json_object &obj);
    bool append(const json_array &obj);

    int size() const;
    void remove(int index);

    QVariant value(int index) const;
    json_object object(int index) const;
    json_array array(int index) const;

    bool replace(int index, const QVariant &value);
    bool replace(int index, const json_object &obj);
    bool replace(int index, const json_array &obj);
};
````
示例代码，更多见demon
```C++
QByteArray json_string = "{\"string\":\"hello world\",\"string_zh\":\"中文乱码\",\"double\":12.32321,\"int\":456,\"bool\":true}";
json_object obj(json_string)
if(obj)
    qDebug() << obj.value("string").toString();
```
