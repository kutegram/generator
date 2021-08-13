#include "method.h"

#include "shared.h"

void writeMethod(QTextStream& header, QTextStream& source, SCHEMA& schema, QString prefix, METHOD m)
{
    header << "void read" << prepareName(prefix + "Method", m.method) << "(TelegramStream &stream, QVariant &i, void* callback = 0);" << endl;
    header << "void write" << prepareName(prefix + "Method", m.method) << "(TelegramStream &stream, QVariant i, void* callback = 0);" << endl;

    source << "void read" << prepareName(prefix + "Method", m.method) << "(TelegramStream &stream, QVariant &i, void* callback)" << endl;
    source << "{" << endl;

    //TODO: flags support
    source << "    QVariant conId;" << endl;
    source << "    readInt32(stream, conId, callback);" << endl;
    source << "    switch (conId.toInt()) {" << endl;
    source << "    case " << QString::number(m.id) << ":" << endl;
    PARAM returnParam = {"i", m.type};
    readParam(source, returnParam, prefix, false, "i");
    source << "    break;" << endl;

    source << "    }" << endl;

    source << "}" << endl;
    source << endl;

    source << "void write" << prepareName(prefix + "Method", m.method) << "(TelegramStream &stream, QVariant i, void* callback)" << endl;
    source << "{" << endl;

    //TODO: flags support
    source << "    TelegramObject obj = i.toMap();" << endl;
    source << "    switch (obj[\"_\"].toInt()) {" << endl;

    source << "    case " << QString::number(m.id) << ":" << endl;
    PARAM id = {"_", "int"};
    source << "    ";
    writeParam(source, id, prefix);
    for (qint32 j = 0; j < m.params.size(); ++j) {
        source << "    ";
        writeParam(source, m.params[j], prefix);
    }
    source << "    break;" << endl;

    source << "    }" << endl;

    source << "}" << endl;
    source << endl;
}
