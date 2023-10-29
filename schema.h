#ifndef SCHEMA_H
#define SCHEMA_H

#include <QStringList>
#include <json.h>

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

struct SCHEMA
{
    QList<CONSTRUCTOR> constructors;
    QList<CONSTRUCTOR> methods;
};

SCHEMA deserializeJSON(QString jsonText);
SCHEMA deserializeTL(QString tlText);

#endif // SCHEMA_H
