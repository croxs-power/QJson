#include "QJson.h"
#include <QDebug>
#include "cJSON.h"

json_interface::json_interface():m_json(NULL)
{
}

json_interface::json_interface(cJSON *json):m_json(NULL)
{
    char *str = cJSON_PrintUnformatted(json);
    if(str != NULL)
    {
        m_json = cJSON_Parse(str);
        free(str);
    }
}

json_interface::json_interface(QByteArray json):m_json(NULL)
{
    if(!json.isEmpty())
        parse(json);
}

json_interface::json_interface(const json_interface *obj):m_json(NULL)
{
    if(obj != NULL && !obj->is_empty())
    {
        parse(obj->json_string());
    }
}

json_interface::json_interface(const json_interface &obj):m_json(NULL)
{
    if(!obj.is_empty())
    {
        parse(obj.json_string());
    }
}

json_interface::~json_interface()
{
    clear();
}

json_interface::operator bool() const
{
    return is_empty()? false : true;
}

bool json_interface::operator !() const
{
    return is_empty() ? true : false ;
}

json_interface &json_interface::operator=(const json_interface &json)
{
    if(!json.is_empty())
    {
        parse(json.json_string());
    }
    return (*this);
}

bool json_interface::operator==(const json_interface &json) const
{
    return (json.json_string() == this->json_string());
}

bool json_interface::parse(QByteArray json_string)
{
    clear();
    m_json = cJSON_Parse((QString::fromUtf8(json_string.data())).toLocal8Bit());
    if(m_json == NULL)
    {
        m_errorMsg = cJSON_GetErrorPtr();
        return  false;
    }
    else
    {
        return true;
    }
}

cJSON *json_interface::parse_create(QByteArray json_string)
{
    cJSON *json = cJSON_Parse((QString::fromUtf8(json_string.data())).toLocal8Bit());
    return  json;
}

bool json_interface::is_empty() const
{
    if(m_json == NULL)
        return  true;

    return (cJSON_GetArraySize(m_json) == 0 ? true : false);
}

QByteArray json_interface::json_string() const
{
    QByteArray ar;
    if(!is_empty())
    {
        char *str = cJSON_PrintUnformatted(m_json);
        ar = QString::fromLocal8Bit(str).toUtf8();
        free(str);
    }
    return  ar;
}

QString json_interface::format_string() const
{
    QString json_str;
    if(!is_empty())
    {
        char *str = cJSON_PrintUnformatted(m_json);
        json_str = QString::fromLocal8Bit(str);
        free(str);
    }
    return  json_str;
}

void json_interface::clear()
{
    if(m_json != NULL)
    {
        cJSON_Delete(m_json);
        m_json = NULL;
    }
}

cJSON *json_interface::internal_object() const
{
    return m_json;
}

void json_interface::create(json_type_enum type)
{
    if(m_json != NULL)
        return;

    if(type == json_type_object)
    {
        m_json = cJSON_CreateObject();
    }
    else
    {
        m_json = cJSON_CreateArray();
    }
}

json_value::json_value():m_type(Type_Unvalid)
{

}

json_value::json_value(bool b)
{
    m_type = Type_Bool;
    m_value = b;
}

json_value::json_value(double n)
{
    m_type = Type_Double;
    m_value = n;
}

json_value::json_value(const QString &s)
{
    m_type = Type_String;
    m_value = s;
}

json_value::json_value(const char *s)
{
    m_type = Type_String;
    m_value = (s);
}

json_value::json_value(const json_value &other)
{
    m_type = other.m_type;
    m_value = other.m_value;
}

json_value::json_value(int n)
{
    m_type = Type_Int;
    m_value = n;
}

json_value::json_value(qint64 n)
{
    m_type = Type_Int;
    m_value = n;
}

json_value::json_value(const json_object &a)
{
    if(a)
    {
        m_type = Type_Object;
        m_value = a.json_string();
    }
}

json_value::json_value(const json_array &a)
{
    if(a)
    {
        m_type = Type_Array;
        m_value = a.json_string();
    }
}

bool json_value::toBool() const
{
    return m_value.toBool();
}

double json_value::toDouble() const
{
    return m_value.toDouble();
}

int json_value::toInt() const
{
    return m_value.toInt();
}

QString json_value::toString() const
{
    return m_value.toString();
}

json_object json_value::toObject() const
{
    return json_object(m_value.toByteArray());
}

json_array json_value::toArray() const
{
    return json_array(m_value.toByteArray());
}

json_value::Type_Enum json_value::type() const
{
    return m_type;
}

bool json_value::is_empty() const
{
    return m_value.isNull();
}

json_object::json_object():json_interface()
{

}

json_object::json_object(cJSON *json):json_interface(json)
{

}

json_object::json_object(QByteArray json):json_interface(json)
{

}

json_object::json_object(const json_object *obj):json_interface(obj)
{

}

json_object::json_object(const json_object &obj):json_interface(obj)
{

}

bool json_object::insert(const QString &key, const json_value &value)
{
    if(value.is_empty())
        return  false;

    create(json_type_object);

    bool ret = true;
    switch (value.type())
    {
    case json_value::Type_Int:
    {
        if(exist(key))
        {
            cJSON *item = cJSON_CreateNumber(value.toInt());
            cJSON_ReplaceItemInObject(m_json, key.toStdString().c_str(), item);
        }
        else
        {
            cJSON *item = cJSON_CreateNumber(value.toInt());
            cJSON_AddItemToObject(m_json, key.toStdString().c_str(), item);
        }
    }
        break;
    case json_value::Type_Double:
    {
        if(exist(key))
        {
            cJSON *item = cJSON_CreateNumber(value.toDouble());
            cJSON_ReplaceItemInObject(m_json, key.toStdString().c_str(), item);
        }
        else
        {
            cJSON *item = cJSON_CreateNumber(value.toDouble());
            cJSON_AddItemToObject(m_json, key.toStdString().c_str(), item);
        }
    }
        break;
    case json_value::Type_String:
    {
        if(exist(key))
        {
            cJSON *item = cJSON_CreateString(value.toString().toLocal8Bit().data());
            cJSON_ReplaceItemInObject(m_json, key.toStdString().c_str(), item);
        }
        else
        {
            cJSON *item = cJSON_CreateString(value.toString().toLocal8Bit().data());
            cJSON_AddItemToObject(m_json, key.toStdString().c_str(), item);
        }
    }
        break;
    case json_value::Type_Bool:
    {
        if(exist(key))
        {
            cJSON *item = cJSON_CreateBool(value.toBool());
            cJSON_ReplaceItemInObject(m_json, key.toStdString().c_str(), item);
        }
        else
        {
            cJSON *item = cJSON_CreateBool(value.toBool());
            cJSON_AddItemToObject(m_json, key.toStdString().c_str(), item);
        }
    }
        break;
    case json_value::Type_Object:
    {
        cJSON *item = parse_create(value.toObject().json_string());
        if(item != NULL)
        {
            if(exist(key))
            {
                cJSON_ReplaceItemInObject(m_json, key.toStdString().c_str(), item);
            }
            else
            {
                cJSON_AddItemToObject(m_json, key.toStdString().c_str(), item);
            }
        }
        else
        {
            m_errorMsg = "insert object is null";
            ret = false;
        }
    }
        break;
    case json_value::Type_Array:
    {
        cJSON *item = parse_create(value.toArray().json_string());
        if(item != NULL)
        {
            if(exist(key))
            {
                cJSON_ReplaceItemInObject(m_json, key.toStdString().c_str(), item);
            }
            else
            {
                cJSON_AddItemToObject(m_json, key.toStdString().c_str(), item);
            }
        }
        else
        {
            m_errorMsg = "insert array is null";
            ret = false;
        }
    }
        break;
    default:
    {
        m_errorMsg = QObject::tr("Unsupported data format:%1").arg(value.type());
        qDebug() << "json_object" << m_errorMsg;
        ret = false;
    }
        break;
    }

    return  ret;
}

json_value json_object::value(const QString &key)
{
    json_value value;

    if(!is_empty() && exist(key))
    {
        cJSON *item = cJSON_GetObjectItem(m_json, key.toStdString().c_str());
        if(cJSON_IsTrue(item))
        {
            value = true;
        }
        else if(cJSON_IsFalse(item))
        {
            value = false;
        }
        else if(cJSON_IsNumber(item))
        {
            value = item->valuedouble;
        }
        else if(cJSON_IsString(item))
        {
            value = QString::fromLocal8Bit(item->valuestring);
        }
        else if(cJSON_IsObject(item))
        {
            cJSON *item = cJSON_GetObjectItem(m_json, key.toStdString().c_str());
            value = json_object(item);
        }
        else if(cJSON_IsArray(item))
        {
            cJSON *item = cJSON_GetObjectItem(m_json, key.toStdString().c_str());
            value = json_array(item);
        }
    }
    return  value;
}

json_object json_object::object(const QString &key)
{
    if(is_empty() || !exist(key))
    {
        return json_object();
    }

    cJSON *item = cJSON_GetObjectItem(m_json, key.toStdString().c_str());
    if(item == NULL || item->type != cJSON_Object)
        return  json_object();

    return json_object(item);
}

json_array json_object::array(const QString &key)
{
    if(is_empty() || !exist(key))
    {
        return json_array();
    }

    cJSON *item = cJSON_GetObjectItem(m_json, key.toStdString().c_str());
    if(item == NULL || item->type != cJSON_Array)
        return  json_array();

    return json_array(item);
}

QStringList json_object::keys()
{
    QStringList keyList;
    if(!is_empty())
    {
        int num = cJSON_GetArraySize(m_json);
        for (int i = 0; i < num; i++)
        {
            cJSON * item = cJSON_GetArrayItem(m_json, i);
            keyList.append(item->string);
        }
    }
    return  keyList;
}

bool json_object::exist(QString key)
{
    if(is_empty())
        return  false;

    return (cJSON_HasObjectItem(m_json, key.toStdString().c_str()) == 1 );
}

void json_object::remove(const QString &key)
{
    if(is_empty())
        return;

    cJSON_DeleteItemFromObject(m_json, key.toStdString().c_str());
}

json_array::json_array():json_interface()
{

}

json_array::json_array(cJSON *json):json_interface(json)
{

}

json_array::json_array(QByteArray json):json_interface(json)
{

}

json_array::json_array(const json_array *obj):json_interface(obj)
{

}

json_array::json_array(const json_array &obj):json_interface(obj)
{

}

bool json_array::append(const json_value &value)
{
    create(json_type_array);

    bool ret = true;
    switch (value.type())
    {
    case json_value::Type_Int:
    {
        cJSON *item = cJSON_CreateNumber(value.toInt());
        cJSON_AddItemToArray(m_json, item);
    }
        break;
    case json_value::Type_Double:
    {
        cJSON *item = cJSON_CreateNumber(value.toDouble());
        cJSON_AddItemToArray(m_json, item);
    }
        break;
    case json_value::Type_String:
    {
        cJSON *item = cJSON_CreateString(value.toString().toLocal8Bit().data());
        cJSON_AddItemToArray(m_json, item);
    }
        break;
    case json_value::Type_Bool:
    {
        cJSON *item = cJSON_CreateBool(value.toBool());
        cJSON_AddItemToArray(m_json, item);
    }
        break;
    case json_value::Type_Object:
    {
        cJSON *item = parse_create(value.toObject().json_string());
        if(item != NULL)
        {
            cJSON_AddItemToArray(m_json, item);
        }
        else
        {
            ret = false;
            m_errorMsg = "append object is null";
        }
    }
        break;
    case json_value::Type_Array:
    {
        cJSON *item = parse_create(value.toArray().json_string());
        if(item != NULL)
        {
            cJSON_AddItemToArray(m_json, item);
        }
        else
        {
            ret = false;
            m_errorMsg = "append array is null";
        }
    }
        break;
    default:
    {
        m_errorMsg = QObject::tr("Unsupported data format:%1").arg(value.type());
        qDebug() << "json_array" << m_errorMsg;
        ret = false;
    }
        break;
    }

    return  ret;
}

int json_array::size() const
{
    if(is_empty())
        return 0;

    return cJSON_GetArraySize(m_json);
}

void json_array::remove(int index)
{
    if(is_empty())
        return;

    if(index > -1 && index < size())
    {
        cJSON_DeleteItemFromArray(m_json, index);
    }
}

json_value json_array::value(int index) const
{
    json_value value;
    if(is_empty())
        return value;

    cJSON *item = cJSON_GetArrayItem(m_json, index);
    if(item == NULL)
        return value;

    if(cJSON_IsTrue(item))
    {
        value = true;
    }
    else if(cJSON_IsFalse(item))
    {
        value = false;
    }
    else if(cJSON_IsNumber(item))
    {
        value = item->valuedouble;
    }
    else if(cJSON_IsString(item))
    {
        value = QString::fromLocal8Bit(item->valuestring);
    }
    else if(cJSON_IsObject(item))
    {
        value = json_object(item);
    }
    else if(cJSON_IsArray(item))
    {
        value = json_array(item);
    }

    return value;
}

json_object json_array::object(int index) const
{
    json_object obj;
    if(is_empty())
        return obj;

    cJSON *item = cJSON_GetArrayItem(m_json, index);
    if(item == NULL)
        return obj;

    if(item->type != cJSON_Object)
        return obj;

    return json_object(item);
}

json_array json_array::array(int index) const
{
    json_array obj;
    if(is_empty())
        return obj;

    cJSON *item = cJSON_GetArrayItem(m_json, index);
    if(item == NULL)
        return obj;

    if(item->type != cJSON_Array)
        return obj;

    return json_array(item);
}

json_value json_array::first() const
{
    if(is_empty())
        return json_value();
    else
        return value(0);
}

json_value json_array::last() const
{
    if(is_empty())
        return json_value();
    else
        return value(size() - 1);
}

bool json_array::replace(int index, const json_value &value)
{
    if(is_empty())
        return false;

    if(index < 0 || index >= size())
        return  false;

    cJSON *item = NULL;
    switch (value.type())
    {
    case json_value::Type_Int:
    {
        item = cJSON_CreateNumber(value.toInt());
    }
        break;
    case json_value::Type_Double:
    {
        item = cJSON_CreateNumber(value.toDouble());
    }
        break;
    case json_value::Type_String:
    {
        item = cJSON_CreateString(value.toString().toLocal8Bit().data());
    }
        break;
    case json_value::Type_Bool:
    {
        item = cJSON_CreateBool(value.toBool());
    }
        break;
    case json_value::Type_Object:
    {
        item = parse_create(value.toObject().json_string());
    }
        break;
    case json_value::Type_Array:
    {
        item = parse_create(value.toArray().json_string());
    }
        break;
    default:
    {
        m_errorMsg = QObject::tr("Unsupported data format:%1").arg(value.type());
        qDebug() << "json_array" << m_errorMsg;
    }
        break;
    }
    if(item == NULL)
        return  false;

    return cJSON_ReplaceItemInArray(m_json, index, item);
}
