#include "constructor.h"

#include "shared.h"

void writeInterfaceHeader(QTextStream &header, SCHEMA &schema, QString prefix, QString interface, bool forward)
{
    if (forward) {
        header << "template <READ_METHOD R, WRITE_METHOD W> ";
        header << "void read" << prepareName(prefix, interface) << "(TelegramStream &stream, QVariant &i, void* callback = 0);" << endl;
        header << "template <READ_METHOD R, WRITE_METHOD W> ";
        header << "void write" << prepareName(prefix, interface) << "(TelegramStream &stream, QVariant i, void* callback = 0);" << endl;
        return;
    }

    QList<CONSTRUCTOR> predicts;
    for (qint32 i = 0; i < schema.constructors.size(); ++i) {
        if (schema.constructors[i].type == interface) predicts.append(schema.constructors[i]);
    }

    header << "template <READ_METHOD R, WRITE_METHOD W> ";
    header << "void read" << prepareName(prefix, interface) << "(TelegramStream &stream, QVariant &i, void* callback = 0)" << endl;
    header << "{" << endl;

    header << "    TelegramObject obj;" << endl;
    header << "    QVariant conId;" << endl;
    if (interface.replace("%", "") != "Message" || prefix.toLower() != "mt") header << "    readInt32(stream, conId, callback);" << endl;
    header << "    switch (conId.toInt()) {" << endl;
    for (qint32 i = 0; i < predicts.size(); ++i) {
        CONSTRUCTOR c = predicts[i];
        header << "    case " << QString::number(c.id) << ":" << endl;
        header << "        obj[\"_\"] = conId.toInt();" << endl;
        for (qint32 j = 0; j < c.params.size(); ++j) {
            header << "    ";
            readParam(header, c.params, c.params[j], prefix);
        }
        header << "    break;" << endl;
    }
    header << "    }" << endl;
    header << "    i = obj;" << endl;

    header << "}" << endl;
    header << endl;

    header << "template <READ_METHOD R, WRITE_METHOD W> ";
    header << "void write" << prepareName(prefix, interface) << "(TelegramStream &stream, QVariant i, void* callback = 0)" << endl;
    header << "{" << endl;

    header << "    TelegramObject obj = i.toMap();" << endl;
    header << "    switch (obj[\"_\"].toInt()) {" << endl;
    for (qint32 i = 0; i < predicts.size(); ++i) {
        CONSTRUCTOR c = predicts[i];
        header << "    case " << QString::number(c.id) << ":" << endl;
        PARAM id = {"_", "int"};
        header << "    ";
        if (interface.replace("%", "") != "Message" || prefix.toLower() != "mt") writeParam(header, c.params, id, prefix);
        for (qint32 j = 0; j < c.params.size(); ++j) {
            if (c.params[j].type.split("?").last().toLower() != "true") header << "    ";
            writeParam(header, c.params, c.params[j], prefix);
        }
        header << "    break;" << endl;
    }
    header << "    }" << endl;

    header << "}" << endl;
    header << endl;
}

void writeInterface(QTextStream& header, QTextStream& source, SCHEMA& schema, QString prefix, QString interface, bool forward)
{
    QList<CONSTRUCTOR> predicts;
    for (qint32 i = 0; i < schema.constructors.size(); ++i) {
        if (schema.constructors[i].type == interface) predicts.append(schema.constructors[i]);
    }

    for (qint32 i = 0; i < predicts.size(); ++i) {
        for (qint32 j = 0; j < predicts[i].params.size(); ++j) {
            QString paramType = predicts[i].params[j].type.split("?").last().toLower();
            if (paramType == "object" || ((paramType.replace("%", "") == "message" || paramType.replace("%", "") == "vector<message>") && prefix.toLower() == "mt")) {
                writeInterfaceHeader(header, schema, prefix, interface, forward);
                return;
            }
        }
    }
    if (forward) return;

    header << "void read" << prepareName(prefix, interface) << "(TelegramStream &stream, QVariant &i, void* callback = 0);" << endl;
    header << "void write" << prepareName(prefix, interface) << "(TelegramStream &stream, QVariant i, void* callback = 0);" << endl;

    source << "void read" << prepareName(prefix, interface) << "(TelegramStream &stream, QVariant &i, void* callback)" << endl;
    source << "{" << endl;

    source << "    TelegramObject obj;" << endl;
    source << "    QVariant conId;" << endl;
    if (interface.replace("%", "") != "Message" || prefix.toLower() != "mt") source << "    readInt32(stream, conId, callback);" << endl;
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
        if (interface.replace("%", "") != "Message" || prefix.toLower() != "mt") writeParam(source, c.params, id, prefix);
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
