#include "constructor.h"

#include "shared.h"

void writeInterface(QTextStream& header, QTextStream& source, SCHEMA& schema, QString prefix, QString interface)
{
    header << "void read" << prepareName(prefix, interface) << "(TelegramStream &stream, QtJson::JsonObject &obj);" << endl;
    header << "void write" << prepareName(prefix, interface) << "(TelegramStream &stream, QtJson::JsonObject &obj);" << endl;

    QList<CONSTRUCTOR> predicts;
    for (qint32 i = 0; i < schema.constructors.size(); ++i) {
        if (schema.constructors[i].type == interface) predicts.append(schema.constructors[i]);
    }

    source << "void read" << prepareName(prefix, interface) << "(TelegramStream &stream, QtJson::JsonObject &obj)" << endl;
    source << "{" << endl;

    //TODO: flags support
    source << "    qint32 conId;" << endl;
    source << "    stream >> conId;" << endl;
    source << "    switch (conId) {" << endl;
    for (qint32 i = 0; i < predicts.size(); ++i) {
        CONSTRUCTOR c = predicts[i];
        source << "    case " << QString::number(c.id) << ":" << endl;
        source << "    obj[\"id\"] = conId;" << endl;
        for (qint32 j = 0; j < c.params.size(); ++j) {
            source << "    ";
            readParam(source, c.params[j], prefix);
        }
        source << "    break;" << endl;
    }
    source << "    }" << endl;

    source << "}" << endl;
    source << endl;

    source << "void write" << prepareName(prefix, interface) << "(TelegramStream &stream, QtJson::JsonObject &obj)" << endl;
    source << "{" << endl;

    //TODO: flags support
    source << "    switch (obj[\"id\"].toInt()) {" << endl;
    for (qint32 i = 0; i < predicts.size(); ++i) {
        CONSTRUCTOR c = predicts[i];
        source << "    case " << QString::number(c.id) << ":" << endl;
        source << "        stream << obj[\"id\"].toInt();" << endl;
        for (qint32 j = 0; j < c.params.size(); ++j) {
            source << "    ";
            writeParam(source, c.params[j], prefix);
        }
        source << "    break;" << endl;
    }
    source << "    }" << endl;

    source << "}" << endl;
    source << endl;
}
