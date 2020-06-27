#ifndef QJSON_HEADER_20200623
#define QJSON_HEADER_20200623

#include <QObject>
#include <QVariant>
#include <QStringList>
#include <QByteArray>

struct cJSON;
class json_object;
class json_array;
class json_attribute;

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

#endif // PUGIJSON_H
