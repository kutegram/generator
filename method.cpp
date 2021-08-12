#include "method.h"

#include "shared.h"

void writeMethod(QTextStream& header, QTextStream& source, SCHEMA& schema, QString prefix, METHOD m)
{
    header << "void read" << prepareName(prefix, m.method) << "(TelegramStream &stream, QVariant &i);" << endl;
    header << "void write" << prepareName(prefix, m.method) << "(TelegramStream &stream, QVariant i);" << endl;

    source << "void read" << prepareName(prefix, m.method) << "(TelegramStream &stream, QVariant &i)" << endl;
    source << "{" << endl;

    //TODO: flags support
    source << "    QVariant conId;" << endl;
    source << "    readInt32(stream, conId);" << endl;
    source << "    switch (conId.toInt()) {" << endl;
    source << "    case " << QString::number(m.id) << ":" << endl;
    PARAM returnParam = {"conId", m.type};
    readParam(source, returnParam, prefix, false, "i");
    source << "    break;" << endl;

    source << "    }" << endl;

    source << "}" << endl;
    source << endl;

    source << "void write" << prepareName(prefix, m.method) << "(TelegramStream &stream, QVariant i)" << endl;
    source << "{" << endl;

    //TODO: flags support
    source << "    TelegramObject obj = i.toMap();" << endl;
    source << "    switch (obj[\"id\"].toInt()) {" << endl;

    source << "    case " << QString::number(m.id) << ":" << endl;
    PARAM id = {"id", "int"};
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
