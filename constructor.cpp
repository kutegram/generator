#include "constructor.h"

#include "shared.h"

void writeInterface(QTextStream& header, QTextStream& source, SCHEMA& schema, QString prefix, QString interface)
{
    header << "void read" << prepareName(prefix, interface) << "(TelegramStream &stream, QVariant &i, void* callback = 0);" << endl;
    header << "void write" << prepareName(prefix, interface) << "(TelegramStream &stream, QVariant i, void* callback = 0);" << endl;

    QList<CONSTRUCTOR> predicts;
    for (qint32 i = 0; i < schema.constructors.size(); ++i) {
        if (schema.constructors[i].type == interface) predicts.append(schema.constructors[i]);
    }

    source << "void read" << prepareName(prefix, interface) << "(TelegramStream &stream, QVariant &i, void* callback)" << endl;
    source << "{" << endl;

    source << "    TelegramObject obj;" << endl;
    source << "    QVariant conId;" << endl;
    source << "    readInt32(stream, conId, callback);" << endl;
    source << "    switch (conId.toInt()) {" << endl;
    for (qint32 i = 0; i < predicts.size(); ++i) {
        CONSTRUCTOR c = predicts[i];
        source << "    case " << QString::number(c.id) << ":" << endl;
        source << "        obj[\"_\"] = conId.toInt();" << endl;
        for (qint32 j = 0; j < c.params.size(); ++j) {
            source << "    ";
            readParam(source, c.params, c.params[j], prefix);
        }
        source << "    break;" << endl;
    }
    source << "    }" << endl;
    source << "    i = obj;" << endl;

    source << "}" << endl;
    source << endl;

    source << "void write" << prepareName(prefix, interface) << "(TelegramStream &stream, QVariant i, void* callback)" << endl;
    source << "{" << endl;

    source << "    TelegramObject obj = i.toMap();" << endl;
    source << "    switch (obj[\"_\"].toInt()) {" << endl;
    for (qint32 i = 0; i < predicts.size(); ++i) {
        CONSTRUCTOR c = predicts[i];
        source << "    case " << QString::number(c.id) << ":" << endl;
        PARAM id = {"_", "int"};
        source << "    ";
        writeParam(source, c.params, id, prefix);
        for (qint32 j = 0; j < c.params.size(); ++j) {
            if (c.params[j].type.split("?").last().toLower() != "true") source << "    ";
            writeParam(source, c.params, c.params[j], prefix);
        }
        source << "    break;" << endl;
    }
    source << "    }" << endl;

    source << "}" << endl;
    source << endl;
}
