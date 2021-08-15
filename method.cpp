#include "method.h"

#include "shared.h"

void writeMethodHeader(QTextStream& header, SCHEMA& schema, QString prefix, METHOD m, bool forward)
{
    if (forward) {
        header << "template <READ_METHOD R, WRITE_METHOD W> ";
        header << "void read" << prepareName(prefix + "Method", m.method) << "(TelegramStream &stream, QVariant &i, void* callback = 0);" << endl;
        header << "template <READ_METHOD R, WRITE_METHOD W> ";
        header << "void write" << prepareName(prefix + "Method", m.method) << "(TelegramStream &stream, QVariant i, void* callback = 0);" << endl;
        return;
    }

    header << "template <READ_METHOD R, WRITE_METHOD W> ";
    header << "void read" << prepareName(prefix + "Method", m.method) << "(TelegramStream &stream, QVariant &i, void* callback = 0)" << endl;
    header << "{" << endl;

    header << "    QVariant conId;" << endl;
    header << "    readInt32(stream, conId, callback);" << endl;
    header << "    switch (conId.toInt()) {" << endl;
    header << "    case " << QString::number(m.id) << ":" << endl;
    PARAM returnParam = {"i", m.type};
    readParam(header, m.params, returnParam, prefix, false, "i");
    header << "    break;" << endl;

    header << "    }" << endl;

    header << "}" << endl;
    header << endl;

    header << "template <READ_METHOD R, WRITE_METHOD W> ";
    header << "void write" << prepareName(prefix + "Method", m.method) << "(TelegramStream &stream, QVariant i, void* callback = 0)" << endl;
    header << "{" << endl;

    header << "    TelegramObject obj = i.toMap();" << endl;
    header << "    switch (obj[\"_\"].toInt()) {" << endl;

    header << "    case " << QString::number(m.id) << ":" << endl;
    PARAM id = {"_", "int"};
    header << "    ";
    writeParam(header, m.params, id, prefix);
    for (qint32 j = 0; j < m.params.size(); ++j) {
        if (m.params[j].type.split("?").last().toLower() != "true") header << "    ";
        writeParam(header, m.params, m.params[j], prefix);
    }
    header << "    break;" << endl;

    header << "    }" << endl;

    header << "}" << endl;
    header << endl;
}

void writeMethod(QTextStream& header, QTextStream& source, SCHEMA& schema, QString prefix, METHOD m, bool forward)
{
    if (m.type == "X") {
        writeMethodHeader(header, schema, prefix, m, forward);
        return;
    }
    if (forward) return;

    header << "void read" << prepareName(prefix + "Method", m.method) << "(TelegramStream &stream, QVariant &i, void* callback = 0);" << endl;
    header << "void write" << prepareName(prefix + "Method", m.method) << "(TelegramStream &stream, QVariant i, void* callback = 0);" << endl;

    source << "void read" << prepareName(prefix + "Method", m.method) << "(TelegramStream &stream, QVariant &i, void* callback)" << endl;
    source << "{" << endl;

    source << "    QVariant conId;" << endl;
    source << "    readInt32(stream, conId, callback);" << endl;
    source << "    switch (conId.toInt()) {" << endl;
    source << "    case " << QString::number(m.id) << ":" << endl;
    PARAM returnParam = {"i", m.type};
    readParam(source, m.params, returnParam, prefix, false, "i");
    source << "    break;" << endl;

    source << "    }" << endl;

    source << "}" << endl;
    source << endl;

    source << "void write" << prepareName(prefix + "Method", m.method) << "(TelegramStream &stream, QVariant i, void* callback)" << endl;
    source << "{" << endl;

    source << "    TelegramObject obj = i.toMap();" << endl;
    source << "    switch (obj[\"_\"].toInt()) {" << endl;

    source << "    case " << QString::number(m.id) << ":" << endl;
    PARAM id = {"_", "int"};
    source << "    ";
    writeParam(source, m.params, id, prefix);
    for (qint32 j = 0; j < m.params.size(); ++j) {
        if (m.params[j].type.split("?").last().toLower() != "true") source << "    ";
        writeParam(source, m.params, m.params[j], prefix);
    }
    source << "    break;" << endl;

    source << "    }" << endl;

    source << "}" << endl;
    source << endl;
}
