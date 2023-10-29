#include "generator.h"

void writeEnum(QTextStream& header, SCHEMA& schema, QString prefix)
{
    header << "namespace " << prefix << "Type {" << endl;
    header << "enum Types" << endl;
    header << "{" << endl;
    header << "    Unknown = 0," << endl;

    for (qint32 i = 0; i < schema.constructors.size(); ++i) {
        QString hexId = QString::number((quint32) schema.constructors[i].id, 16);
        while (hexId.length() < 8)
            hexId.prepend("0");
        header << "    " << prepareName("", schema.constructors[i].predicate) << " = " << QString::number(schema.constructors[i].id) << ", //0x" << hexId << endl;
    }

    for (qint32 i = 0; i < schema.methods.size(); ++i) {
        QString hexId = QString::number((quint32) schema.methods[i].id, 16);
        while (hexId.length() < 8)
            hexId.prepend("0");
        header << "    " << prepareName("", schema.methods[i].predicate) << "Method = " << QString::number(schema.methods[i].id) << ", //0x" << hexId << endl;
    }

    header << "};" << endl;
    header << "}" << endl;
    header << endl;
}

void generate(QString filePath, QString prefix, qint32 layer, QString streamHeaderPath)
{
    QFile schemaFile(filePath);
    if (!schemaFile.open(QFile::ReadOnly)) return;

    QFile headerFile(prefix.toLower() + "schema.h");
    if (!headerFile.open(QFile::WriteOnly)) return;
    QTextStream header(&headerFile);

    QFile sourceFile(prefix.toLower() + "schema.cpp");
    if (!sourceFile.open(QFile::WriteOnly)) return;
    QTextStream source(&sourceFile);

    QString schemaText = QString::fromUtf8(schemaFile.readAll());
    schemaFile.close();

    SCHEMA schema;
    if (filePath.endsWith("json"))
        schema = deserializeJSON(schemaText);
    else
        schema = deserializeTL(schemaText);

    header << "#ifndef " << prefix.toUpper() << "SCHEMA_H" << endl;
    header << "#define " << prefix.toUpper() << "SCHEMA_H" << endl;
    header << endl;

    header << "//Generated code." << endl;
    header << endl;
    header << "#include \"" << streamHeaderPath << "\"" << endl;
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
