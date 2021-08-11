#include "schema.h"

SCHEMA deserialize(JsonObject json)
{
    SCHEMA schema;

    JsonArray constructors = json["constructors"].toList();
    for (qint32 i = 0; i < constructors.size(); i++) {
        JsonObject constructorJSON = constructors[i].toMap();
        CONSTRUCTOR constructor;
        constructor.id = constructorJSON["id"].toInt();
        constructor.predicate = constructorJSON["predicate"].toString();
        constructor.type = constructorJSON["type"].toString();

        JsonArray params = constructorJSON["params"].toList();
        for (qint32 j = 0; j < params.size(); j++) {
            JsonObject paramJSON = params[j].toMap();
            PARAM param;
            param.name = paramJSON["name"].toString();
            param.type = paramJSON["type"].toString();
            constructor.params.append(param);
        }
        schema.constructors.append(constructor);
    }

    JsonArray methods = json["methods"].toList();
    for (qint32 i = 0; i < methods.size(); i++) {
        JsonObject methodJSON = methods[i].toMap();
        METHOD method;
        method.id = methodJSON["id"].toInt();
        method.method = methodJSON["method"].toString();
        method.type = methodJSON["type"].toString();

        JsonArray params = methodJSON["params"].toList();
        for (qint32 j = 0; j < params.size(); j++) {
            JsonObject paramJSON = params[j].toMap();
            PARAM param;
            param.name = paramJSON["name"].toString();
            param.type = paramJSON["type"].toString();
            method.params.append(param);
        }
        schema.methods.append(method);
    }

    return schema;
}
