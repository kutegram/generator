#include "method.h"

#include "shared.h"

void writeMethod(QTextStream& header, QTextStream& source, SCHEMA& schema, QString prefix, METHOD m)
{
    if (m.type == "X") header << "template <READ_METHOD R, WRITE_METHOD W> ";
    header << "void read" << prepareName(prefix + "Method", m.method) << "(TelegramStream &stream, QVariant &i, void* callback = 0);" << endl;
    if (m.type == "X") header << "template <READ_METHOD R, WRITE_METHOD W> ";
    header << "void write" << prepareName(prefix + "Method", m.method) << "(TelegramStream &stream, QVariant i, void* callback = 0);" << endl;

    if (m.type == "X") source << "template <READ_METHOD R, WRITE_METHOD W> ";
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

    if (m.type == "X") source << "template <READ_METHOD R, WRITE_METHOD W> ";
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
