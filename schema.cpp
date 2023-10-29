#include "schema.h"

#include "crc32.h"
#include <qt-json/json.h>

SCHEMA deserializeJSON(QString jsonText)
{
    JsonObject json = QtJson::parse(jsonText).toMap();
    SCHEMA schema;

    JsonArray constructors = json["constructors"].toList();
    constructors.append(json["methods"].toList());

    for (qint32 i = 0; i < constructors.size(); i++) {
        JsonObject constructorJSON = constructors[i].toMap();
        bool isMethod = !constructorJSON["method"].toString().isEmpty();

        CONSTRUCTOR constructor;
        constructor.id = constructorJSON["id"].toInt();
        if (isMethod)
            constructor.predicate = constructorJSON["method"].toString();
        else
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

        if (isMethod)
            schema.methods.append(constructor);
        else
            schema.constructors.append(constructor);
    }

    return schema;
}

SCHEMA deserializeTL(QString tlText)
{
    tlText.replace("\r\n", "\n") //replace CRLF -> LF
          .replace("\r", "\n") //remaining CR -> LF
          .replace(QRegExp("\\/\\*(\\*(?!\\/)|[^*])*\\*\\/"), " ") //remove block comments
          .replace(QRegExp("\\/\\/[^\\n]*"), " "); //remove line comments
    QStringList lines = tlText.split("\n");

    SCHEMA schema;
    bool methodsSection = false;

    foreach (QString line, lines) {
        line = line.simplified();

        if (line.isEmpty()) {
            continue;
        }

        if (line == "---types---") {
            methodsSection = false;
            continue;
        }
        if (line == "---functions---") {
            methodsSection = true;
            continue;
        }

        QStringList tokens = line.split(" ", QString::SkipEmptyParts);

        if (tokens.isEmpty()) {
            continue;
        }

        CONSTRUCTOR constructor;

        QString fullType = tokens[0];
        qint32 indexOfId = fullType.indexOf("#");

        if (indexOfId == -1) {
            constructor.predicate = fullType;
            constructor.id = CRC32(line.toUtf8());
        } else {
            constructor.predicate = fullType.left(indexOfId);
            constructor.id = fullType.mid(indexOfId + 1).toUInt(0, 16);
        }

        qint32 typeSeparatorIndex = line.indexOf("=");
        if (typeSeparatorIndex != -1) {
            constructor.type = line.mid(typeSeparatorIndex + 1).simplified();
            if (constructor.type.endsWith(";"))
                constructor.type = constructor.type.mid(0, constructor.type.size() - 1);
        }

        foreach (QString token, tokens.mid(1)) {
            if (token == "=") {
                break;
            }

            //Skip generics because we don't need them
            if (token.startsWith("{") && token.endsWith("}")) {
                continue;
            }

            PARAM param;
            qint32 indexOfType = token.indexOf(":");

            //Same for parameters without type
            if (indexOfType == -1) {
                continue;
            }

            param.name = token.left(indexOfType);
            param.type = token.mid(indexOfType + 1);

            constructor.params.append(param);
        }

        if (methodsSection)
            schema.methods.append(constructor);
        else
            schema.constructors.append(constructor);
    }

    return schema;
}
