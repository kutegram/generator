#include "shared.h"

QStringList flagTypes = QStringList() << "bool" << "qint32" << "quint32" << "qint64" << "double";
QStringList keywords = QStringList() << "conId" << "alignas" << "alignof" << "and" << "and_eq" << "asm" << "atomic_cancel" << "atomic_commit" << "atomic_noexcept" << "auto" << "bitand" << "bitor" << "bool" << "break" << "case" << "catch" << "char" << "char8_t" << "char16_t" << "char32_t" << "class" << "compl" << "concept" << "const" << "consteval" << "constexpr" << "constinit" << "const_cast" << "continue" << "co_await" << "co_return" << "co_yield" << "decltype" << "default" << "delete" << "do" << "double" << "dynamic_cast" << "else" << "enum" << "explicit" << "export" << "extern" << "false" << "float" << "for" << "friend" << "goto" << "if" << "inline" << "int" << "long" << "mutable" << "namespace" << "new" << "noexcept" << "not" << "not_eq" << "nullptr" << "operator" << "or" << "or_eq" << "private" << "protected" << "public" << "reflexpr" << "register" << "reinterpret_cast" << "requires" << "return" << "short" << "signed" << "sizeof" << "static" << "static_assert" << "static_cast" << "struct" << "switch" << "synchronized" << "template" << "this" << "thread_local" << "throw" << "true" << "try" << "typedef" << "typeid" << "typename" << "union" << "unsigned" << "using" << "virtual" << "void" << "volatile" << "wchar_t" << "while" << "xor" << "xor_eq";
#define VECTOR_ID "481674261"

QString prepareName(QString prefix, QString raw)
{
    QString name = prefix;
    raw = raw.replace("%", "");
    QStringList split = raw.split(" ", QString::SkipEmptyParts);
    split = split[0].split("_", QString::SkipEmptyParts);
    for (qint32 i = 0; i < split.size(); ++i) {
        QString item = split[i];
        if (!item.isEmpty()) {
            name += item[0].toUpper();
            item.remove(0, 1);
            name += item;
        }
    }
    split = name.split(".", QString::SkipEmptyParts);
    name.clear();
    for (qint32 i = 0; i < split.size(); ++i) {
        QString item = split[i];
        if (!item.isEmpty()) {
            name += item[0].toUpper();
            item.remove(0, 1);
            name += item;
        }
    }
    return name;
}

/* void writeVector(QTextStream &source, PARAM p, QString prefix)
{
    QString type = p.type.split("?")[0];
    type.remove(0, 7);
    type.chop(1);
    source << "    QList<";
    QString input = type.toLower();

    if (input == "#" || input == "int") {
        source << "qint32";
    }
    else if (input == "uint") {
        source << "quint32";
    }
    else if (input == "long") {
        source << "qint64";
    }
    else if (input == "double") {
        source << "double";
    }
    else if (input == "string") {
        source << "QString";
    }
    else if (input == "bytes") {
        source << "QByteArray";
    }
    else if (input == "bool") {
        source << "bool";
    }
    else if (input == "int128") {
        source << "QByteArray";
    }
    else if (input == "int256") {
        source << "QByteArray";
    }
    else if (input == "!x" || input == "x" || input == "object") {
        source << "//Unsupported. (vector x, vector !x, vector object)" << endl;
    }
    else if (input == "httpwait") {
        source << "//Unsupported. (vector httpwait)" << endl;
    }
    else if (input.startsWith("vector<") && input.endsWith(">")) {
        source << "//Unsupported. (vector vector)" << endl;
    }
    else {
        source << prepareName(prefix, type);
    }

    QString varName = p.name;
    if (keywords.contains(varName)) varName += "_var";

    source << "> " << varName << " = obj[\"" << p.name << "\"].toList();" << endl;

    //write vector header
    source << "        stream << " << VECTOR_ID << ";" << endl;
    source << "        stream << " << varName << ".size();" << endl;

    //"for" cycle
    source << "        for (qint32 i = 0; i < " << varName << "_var";
    source << ".size(); ++i)" << endl;

    source << "        ";

    writeParam(source, p, prefix, varName + "[i]");
} */

void writeParam(QTextStream &source, PARAM p, QString prefix, bool signature, QString replace)
{
    QString input = p.type.split("?")[0].toLower();
    QString dest = replace.isEmpty() ? "obj[\"" + p.name + "\"]" : replace;
    if (!signature) source << "    ";
    else source << "(void*) &";
    if (input == "#" || input == "int") {
        source << "writeInt32";
        source << "(stream, " << dest << ");" << endl;
    }
    else if (input == "uint") {
        source << "writeUInt32";
        if (signature) return;
        source << "(stream, " << dest << ");" << endl;
    }
    else if (input == "long") {
        source << "writeInt64";
        if (signature) return;
        source << "(stream, " << dest << ");" << endl;
    }
    else if (input == "double") {
        source << "writeDouble";
        if (signature) return;
        source << "(stream, " << dest << ");" << endl;
    }
    else if (input == "string") {
        source << "writeString";
        if (signature) return;
        source << "(stream, " << dest << ");" << endl;
    }
    else if (input == "bytes") {
        source << "writeByteArray";
        if (signature) return;
        source << "(stream, " << dest << ");" << endl;
    }
    else if (input == "bool") {
        source << "writeBool";
        if (signature) return;
        source << "(stream, " << dest << ");" << endl;
    }
    else if (input == "int128") {
        source << "writeInt128";
        if (signature) return;
        source << "(stream, " << dest << ");" << endl;
    }
    else if (input == "int256") {
        source << "writeInt256";
        if (signature) return;
        source << "(stream, " << dest << ");" << endl;
    }
    else if (input == "!x" || input == "x" || input == "object") {
        if (signature) {
            source << "0";
            return;
        }
        source << "//Unsupported. (x, !x, object)" << endl;
    }
    else if (input == "httpwait") {
        if (signature) {
            source << "0";
            return;
        }
        source << "//Unsupported. (httpwait)" << endl;
    }
    else if (input.startsWith("vector<") && input.endsWith(">")) {
        //writeVector(source, p, prefix);
        source << "writeVector";
        if (signature) return;
        source << "(stream, " << dest << ", ";
        QString type = p.type.split("?")[0];
        type.remove(0, 7);
        type.chop(1);
        PARAM param = {"item", type};
        writeParam(source, param, prefix, true);
        source << ");" << endl;
    }
    else {
        source << "write" << prepareName(prefix, p.type.split("?")[0]);
        if (signature) return;
        source << "(stream, " << dest << ");" << endl;
    }
}

void readParam(QTextStream &source, PARAM p, QString prefix, bool signature, QString replace)
{
    QString input = p.type.split("?")[0].toLower();
    QString dest = replace.isEmpty() ? "obj[\"" + p.name + "\"]" : replace;
    if (!signature) source << "    ";
    else source << "(void*) &";
    if (input == "#" || input == "int") {
        source << "readInt32";
        source << "(stream, " << dest << ");" << endl;
    }
    else if (input == "uint") {
        source << "readUInt32";
        if (signature) return;
        source << "(stream, " << dest << ");" << endl;
    }
    else if (input == "long") {
        source << "readInt64";
        if (signature) return;
        source << "(stream, " << dest << ");" << endl;
    }
    else if (input == "double") {
        source << "readDouble";
        if (signature) return;
        source << "(stream, " << dest << ");" << endl;
    }
    else if (input == "string") {
        source << "readString";
        if (signature) return;
        source << "(stream, " << dest << ");" << endl;
    }
    else if (input == "bytes") {
        source << "readByteArray";
        if (signature) return;
        source << "(stream, " << dest << ");" << endl;
    }
    else if (input == "bool") {
        source << "readBool";
        if (signature) return;
        source << "(stream, " << dest << ");" << endl;
    }
    else if (input == "int128") {
        source << "readInt128";
        if (signature) return;
        source << "(stream, " << dest << ");" << endl;
    }
    else if (input == "int256") {
        source << "readInt256";
        if (signature) return;
        source << "(stream, " << dest << ");" << endl;
    }
    else if (input == "!x" || input == "x" || input == "object") {
        if (signature) {
            source << "0";
            return;
        }
        source << "//Unsupported. (x, !x, object)" << endl;
    }
    else if (input == "httpwait") {
        if (signature) {
            source << "0";
            return;
        }
        source << "//Unsupported. (httpwait)" << endl;
    }
    else if (input.startsWith("vector<") && input.endsWith(">")) {
        //readVector(source, p, prefix);
        source << "readVector";
        if (signature) return;
        source << "(stream, " << dest << ", ";
        QString type = p.type.split("?")[0];
        type.remove(0, 7);
        type.chop(1);
        PARAM param = {"item", type};
        readParam(source, param, prefix, true);
        source << ");" << endl;
    }
    else {
        source << "read" << prepareName(prefix, p.type.split("?")[0]);
        if (signature) return;
        source << "(stream, " << dest << ");" << endl;
    }
}
