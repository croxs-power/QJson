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

json_interface::json_interface(const json_interface *obj)
{
    if(obj != NULL && !obj->is_empty())
    {
        parse(obj->json_string());
    }
}

json_interface::json_interface(const json_interface &obj)
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
        if(m_json != NULL)
            clear();
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
    m_json = cJSON_Parse(json_string.data());
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
        ar = str;
        free(str);
    }
    return  ar;
}

QString json_interface::format_string() const
{
    QString json_str;
    if(!is_empty())
    {
        char *string = cJSON_PrintUnformatted(m_json);
        json_str = QString::fromLocal8Bit(string);
        free(string);
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

void json_interface::Create(json_type_enum type)
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

bool json_object::insert(const QString &key, const QVariant &value)
{
    Create(json_type_object);

    bool ret = true;
    switch (value.type())
    {
    case QVariant::Int:
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
    case QVariant::UInt:
    {
        if(exist(key))
        {
            cJSON *item = cJSON_CreateNumber(value.toUInt());
            cJSON_ReplaceItemInObject(m_json, key.toStdString().c_str(), item);
        }
        else
        {
            cJSON *item = cJSON_CreateNumber(value.toUInt());
            cJSON_AddItemToObject(m_json, key.toStdString().c_str(), item);
        }
    }
        break;
    case 38:
    case QVariant::Double:
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
    case QVariant::String:
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
    case QVariant::Bool:
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

bool json_object::insert(const QString &key, const json_object &value)
{
    if(value.is_empty())
        return false;

    Create(json_type_object);

    cJSON *item = cJSON_Parse(value.json_string().data());
    if(item == NULL)
        return  false;

    if(exist(key))
    {
        cJSON_ReplaceItemInObject(m_json, key.toStdString().c_str(), item);
    }
    else
    {
        cJSON_AddItemToObject(m_json, key.toStdString().c_str(), item);
    }
    return  true;
}

bool json_object::insert(const QString &key, const json_array &value)
{
    if(value.is_empty())
        return false;

    Create(json_type_object);

    cJSON *item = cJSON_Parse(value.json_string().data());
    if(item == NULL)
        return  false;

    if(exist(key))
    {
        cJSON_ReplaceItemInObject(m_json, key.toStdString().c_str(), item);
    }
    else
    {
        cJSON_AddItemToObject(m_json, key.toStdString().c_str(), item);
    }
    return  true;
}

QVariant json_object::value(const QString &key)
{
    QVariant value;

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
            if(item->valuedouble - (int)item->valuedouble > 0)
                value = item->valuedouble;
            else
                value = (int)item->valuedouble;
        }
        else if(cJSON_IsString(item))
        {
            value = QString::fromLocal8Bit(item->valuestring);
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

bool json_array::append(const QVariant &value)
{
    Create(json_type_array);

    bool ret = true;
    switch (value.type())
    {
    case QVariant::Int:
    {
        cJSON *item = cJSON_CreateNumber(value.toInt());
        cJSON_AddItemToArray(m_json, item);
    }
        break;
    case QVariant::UInt:
    {
        cJSON *item = cJSON_CreateNumber(value.toUInt());
        cJSON_AddItemToArray(m_json, item);
    }
        break;
    case 38:
    case QVariant::Double:
    {
        cJSON *item = cJSON_CreateNumber(value.toDouble());
        cJSON_AddItemToArray(m_json, item);
    }
        break;
    case QVariant::String:
    {
        cJSON *item = cJSON_CreateString(value.toString().toLocal8Bit().data());
        cJSON_AddItemToArray(m_json, item);
    }
        break;
    case QVariant::Bool:
    {
        cJSON *item = cJSON_CreateBool(value.toBool());
        cJSON_AddItemToArray(m_json, item);
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

bool json_array::append(const json_object &obj)
{
    cJSON *item = cJSON_Parse(obj.json_string().data());
    if(item == NULL)
        return false;

    Create(json_type_array);

    cJSON_AddItemToArray(m_json, item);
    return  true;
}

bool json_array::append(const json_array &obj)
{
    cJSON *item = cJSON_Parse(obj.json_string().data());
    if(item == NULL)
        return false;

    Create(json_type_array);

    cJSON_AddItemToArray(m_json, item);
    return  true;
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

QVariant json_array::value(int index) const
{
    QVariant value;
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
        if(item->valuedouble - (int)item->valuedouble > 0)
            value = item->valuedouble;
        else
            value = (int)item->valuedouble;
    }
    else if(cJSON_IsString(item))
    {
        value = QString::fromLocal8Bit(item->valuestring);
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

bool json_array::replace(int index, const QVariant &value)
{
    if(is_empty())
        return false;

    if(index < 0 || index >= size())
        return  false;

    cJSON *item = NULL;
    switch (value.type())
    {
    case QVariant::Int:
    {
        item = cJSON_CreateNumber(value.toInt());
    }
        break;
    case QVariant::UInt:
    {
        item = cJSON_CreateNumber(value.toUInt());
    }
        break;
    case 38:
    case QVariant::Double:
    {
        item = cJSON_CreateNumber(value.toDouble());
    }
        break;
    case QVariant::String:
    {
        item = cJSON_CreateString(value.toString().toLocal8Bit().data());
    }
        break;
    case QVariant::Bool:
    {
        item = cJSON_CreateBool(value.toBool());
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

bool json_array::replace(int index, const json_object &obj)
{
    if(is_empty())
        return false;

    if(index < 0 || index >= size())
        return  false;

    cJSON *item = cJSON_Parse(obj.json_string().data());
    if(item == NULL)
        return false;

    return cJSON_ReplaceItemInArray(m_json, index, item);
}

bool json_array::replace(int index, const json_array &obj)
{
    if(is_empty())
        return false;

    if(index < 0 || index >= size())
        return  false;

    cJSON *item = cJSON_Parse(obj.json_string().data());
    if(item == NULL)
        return false;

    return cJSON_ReplaceItemInArray(m_json, index, item);
}
