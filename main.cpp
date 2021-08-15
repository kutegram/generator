#include <QList>
#include <QFile>
#include <QTextStream>
#include <qt-json/json.h>
#include <QStringList>

#include "schema.h"
#include "shared.h"
#include "constructor.h"
#include "method.h"

using namespace QtJson;

void writeEnum(QTextStream& header, SCHEMA& schema, QString prefix)
{
    header << "namespace " << prefix << "Type {" << endl;
    header << "enum Types" << endl;
    header << "{" << endl;

    for (qint32 i = 0; i < schema.constructors.size(); ++i) {
        header << "    " << prepareName("", schema.constructors[i].predicate) << " = " << QString::number(schema.constructors[i].id) << "," << endl;
    }

    for (qint32 i = 0; i < schema.methods.size(); ++i) {
        header << "    " << prepareName("", schema.methods[i].method) << "Method = " << QString::number(schema.methods[i].id) << "," << endl;
    }

    header << "};" << endl;
    header << "}" << endl;
    header << endl;
}

void generate(QString jsonPath, QString prefix, qint32 layer)
{
    QFile jsonFile(jsonPath);
    if (!jsonFile.open(QFile::ReadOnly)) return;

    QFile headerFile(prefix.toLower() + "schema.h");
    if (!headerFile.open(QFile::WriteOnly)) return;
    QTextStream header(&headerFile);

    QFile sourceFile(prefix.toLower() + "schema.cpp");
    if (!sourceFile.open(QFile::WriteOnly)) return;
    QTextStream source(&sourceFile);

    SCHEMA schema = deserialize(parse(QTextStream(&jsonFile).readAll()).toMap());
    jsonFile.close();

    header << "#ifndef " << prefix.toUpper() << "SCHEMA_H" << endl;
    header << "#define " << prefix.toUpper() << "SCHEMA_H" << endl;
    header << endl;

    header << "//Generated code." << endl;
    header << endl;
    header << "#include \"telegramstream.h\"" << endl;
    header << endl;

    source << "//Generated code." << endl;
    source << endl;
    source << "#include \"" << prefix.toLower() << "schema.h\"" << endl;
    source << endl;

    if (layer) {
        header << "#define API_LAYER " << QString::number(layer) << endl;
        header << endl;
    }

    QStringList interfaces;
    for (qint32 i = 0; i < schema.constructors.size(); ++i) {
        if (!interfaces.contains(schema.constructors[i].type)) interfaces.append(schema.constructors[i].type);
    }

    writeEnum(header, schema, prefix);

    for (qint32 i = 0; i < interfaces.size(); ++i) {
        writeInterface(header, source, schema, prefix, interfaces[i], true);
    }

    header << endl;

    for (qint32 i = 0; i < schema.methods.size(); ++i) {
        writeMethod(header, source, schema, prefix, schema.methods[i], true);
    }

    header << endl;

    for (qint32 i = 0; i < interfaces.size(); ++i) {
        writeInterface(header, source, schema, prefix, interfaces[i], false);
    }

    header << endl;

    for (qint32 i = 0; i < schema.methods.size(); ++i) {
        writeMethod(header, source, schema, prefix, schema.methods[i], false);
    }

    header << endl;

    header << "#endif //" << prefix.toUpper() << "SCHEMA_H" << endl;
    header << endl;

    headerFile.close();
    sourceFile.close();
}

int main(int argc, char *argv[])
{
    generate(":/mtproto.json", "MT", 0);
    generate(":/telegram.json", "TL", 121);
    qDebug("Finished.");
}
