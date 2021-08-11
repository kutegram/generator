#ifndef SCHEMA_H
#define SCHEMA_H

#include <QStringList>
#include <qt-json/json.h>

using namespace QtJson;

struct PARAM
{
    QString name;
    QString type;
};

struct CONSTRUCTOR
{
    qint32 id;
    QString predicate;
    QList<PARAM> params;
    QString type;
};

struct METHOD
{
    qint32 id;
    QString method;
    QList<PARAM> params;
    QString type;
};

struct SCHEMA
{
    QList<CONSTRUCTOR> constructors;
    QList<METHOD> methods;
};

SCHEMA deserialize(JsonObject json);

#endif // SCHEMA_H
