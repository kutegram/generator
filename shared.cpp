#include "shared.h"

QStringList flagTypes = QStringList() << "bool" << "qint32" << "quint32" << "qint64" << "double";
QStringList keywords = QStringList() << "R" << "W" << "stream" << "i" << "callback" << "conId" << "alignas" << "alignof" << "and" << "and_eq" << "asm" << "atomic_cancel" << "atomic_commit" << "atomic_noexcept" << "auto" << "bitand" << "bitor" << "bool" << "break" << "case" << "catch" << "char" << "char8_t" << "char16_t" << "char32_t" << "class" << "compl" << "concept" << "const" << "consteval" << "constexpr" << "constinit" << "const_cast" << "continue" << "co_await" << "co_return" << "co_yield" << "decltype" << "default" << "delete" << "do" << "double" << "dynamic_cast" << "else" << "enum" << "explicit" << "export" << "extern" << "false" << "float" << "for" << "friend" << "goto" << "if" << "inline" << "int" << "long" << "mutable" << "namespace" << "new" << "noexcept" << "not" << "not_eq" << "nullptr" << "operator" << "or" << "or_eq" << "private" << "protected" << "public" << "reflexpr" << "register" << "reinterpret_cast" << "requires" << "return" << "short" << "signed" << "sizeof" << "static" << "static_assert" << "static_cast" << "struct" << "switch" << "synchronized" << "template" << "this" << "thread_local" << "throw" << "true" << "try" << "typedef" << "typeid" << "typename" << "union" << "unsigned" << "using" << "virtual" << "void" << "volatile" << "wchar_t" << "while" << "xor" << "xor_eq";
#define VECTOR_ID "481674261"

#include <QtCore>

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

void writeParam(QTextStream &source, QList<PARAM> params, PARAM p, QString prefix, bool signature, QString replace)
{
    QString input = p.type.split("?").last().toLower();
    QString dest = replace.isEmpty() ? "obj[\"" + p.name + "\"]" : replace;
    if (!signature) {
        if (input != "true") {
            source << "    ";
            if (p.type.contains('?')) {
                qint32 parsed = p.type.split("?")[0].split(".")[1].toInt();
                source << "if (obj[\"flags\"].toUInt() & " << QString::number(qPow(2, parsed), 'g', 11) << ") ";
            }
        }
    }
    else source << "(void*) &";
    if (input == "true") {
        if (signature) {
            source << "0";
            return;
        }
    }
    else if (input == "int") {
        source << "writeInt32";
        if (signature) return;
        source << "(stream, " << dest << ", callback);" << endl;
    }
    else if (input == "#" || input == "uint") {
        source << "writeUInt32";
        if (signature) return;
        if (input == "#") {
            source << "(stream, obj[\"flags\"] = (" << endl;
            for (qint32 i = 0; i < params.size(); ++i) {
                PARAM pi = params[i];
                if (!pi.type.contains('?')) continue;
                source << "            (!obj[\"" << pi.name << "\"].isNull()";
                qint32 parsed = pi.type.split("?")[0].split(".")[1].toInt();
                if (parsed) source << " << " << QString::number(parsed);
                source << ") | " << endl;
            }
            source << "        0), callback);" << endl;
        } else {
            source << "(stream, " << dest << ", callback);" << endl;
        }
    }
    else if (input == "long") {
        source << "writeInt64";
        if (signature) return;
        source << "(stream, " << dest << ", callback);" << endl;
    }
    else if (input == "double") {
        source << "writeDouble";
        if (signature) return;
        source << "(stream, " << dest << ", callback);" << endl;
    }
    else if (input == "string") {
        source << "writeString";
        if (signature) return;
        source << "(stream, " << dest << ", callback);" << endl;
    }
    else if (input == "bytes") {
        source << "writeByteArray";
        if (signature) return;
        source << "(stream, " << dest << ", callback);" << endl;
    }
    else if (input == "bool") {
        source << "writeBool";
        if (signature) return;
        source << "(stream, " << dest << ", callback);" << endl;
    }
    else if (input == "int128") {
        source << "writeInt128";
        if (signature) return;
        source << "(stream, " << dest << ", callback);" << endl;
    }
    else if (input == "int256") {
        source << "writeInt256";
        if (signature) return;
        source << "(stream, " << dest << ", callback);" << endl;
    }
    else if (input == "!x" || input == "x") {
        if (signature) {
            source << "W";
            return;
        }
        source << "if (W) (*W)(stream, " << dest << ", callback);" << endl;
    }
    else if (input == "object") {
        if (signature) {
            source << "W";
            return;
        }
        source << "if (W) (*W)(stream, " << dest << ", callback);" << endl;
    }
    else if (input == "httpwait") {
        if (signature) {
            source << "0";
            return;
        }
        //source << "//Unsupported. (httpwait)" << endl;
    }
    else if (input.startsWith("vector<") && input.endsWith(">")) {
        //writeVector(source, p, prefix);
        source << "writeVector";
        if (signature) return;
        source << "(stream, " << dest << ", ";
        QString type = p.type.split("?").last();
        type.remove(0, 7);
        type.chop(1);
        PARAM param = {"item", type};
        writeParam(source, params, param, prefix, true);
        source << ");" << endl;
    }
    else {
        source << "write" << prepareName(prefix, p.type.split("?").last());
        if (input.replace("%", "") == "message" && prefix.toLower() == "mt") source << "<R, W>";
        if (signature) return;
        source << "(stream, " << dest << ", callback);" << endl;
    }
}

void readParam(QTextStream &source, QList<PARAM> params, PARAM p, QString prefix, bool signature, QString replace)
{
    QString input = p.type.split("?").last().toLower();
    QString dest = replace.isEmpty() ? "obj[\"" + p.name + "\"]" : replace;
    if (!signature) {
        source << "    ";
        if (p.type.contains('?')) {
            qint32 parsed = p.type.split("?")[0].split(".")[1].toInt();
            source << "if (obj[\"flags\"].toUInt() & " << QString::number(qPow(2, parsed), 'g', 11) << ") ";
        }
    }
    else source << "(void*) &";
    if (input == "true") {
        if (signature) {
            source << "0";
            return;
        }
        source << dest << " = true;" << endl;
    }
    else if (input == "int") {
        source << "readInt32";
        if (signature) return;
        source << "(stream, " << dest << ", callback);" << endl;
    }
    else if (input == "#" || input == "uint") {
        source << "readUInt32";
        if (signature) return;
        source << "(stream, " << dest << ", callback);" << endl;
    }
    else if (input == "long") {
        source << "readInt64";
        if (signature) return;
        source << "(stream, " << dest << ", callback);" << endl;
    }
    else if (input == "double") {
        source << "readDouble";
        if (signature) return;
        source << "(stream, " << dest << ", callback);" << endl;
    }
    else if (input == "string") {
        source << "readString";
        if (signature) return;
        source << "(stream, " << dest << ", callback);" << endl;
    }
    else if (input == "bytes") {
        source << "readByteArray";
        if (signature) return;
        source << "(stream, " << dest << ", callback);" << endl;
    }
    else if (input == "bool") {
        source << "readBool";
        if (signature) return;
        source << "(stream, " << dest << ", callback);" << endl;
    }
    else if (input == "int128") {
        source << "readInt128";
        if (signature) return;
        source << "(stream, " << dest << ", callback);" << endl;
    }
    else if (input == "int256") {
        source << "readInt256";
        if (signature) return;
        source << "(stream, " << dest << ", callback);" << endl;
    }
    else if (input == "!x" || input == "x") {
        if (signature) {
            source << "R";
            return;
        }
        source << "if (R) (*R)(stream, " << dest << ", callback);" << endl;
    }
    else if (input == "object") {
        if (signature) {
            source << "R";
            return;
        }
        source << "if (R) (*R)(stream, " << dest << ", callback);" << endl;
    }
    else if (input == "httpwait") {
        if (signature) {
            source << "0";
            return;
        }
        //source << "//Unsupported. (httpwait)" << endl;
    }
    else if (input.startsWith("vector<") && input.endsWith(">")) {
        //readVector(source, p, prefix);
        source << "readVector";
        if (signature) return;
        source << "(stream, " << dest << ", ";
        QString type = p.type.split("?").last();
        type.remove(0, 7);
        type.chop(1);
        PARAM param = {"item", type};
        readParam(source, params, param, prefix, true);
        source << ");" << endl;
    }
    else {
        source << "read" << prepareName(prefix, p.type.split("?").last());
        if (input.replace("%", "") == "message" && prefix.toLower() == "mt") source << "<R, W>";
        if (signature) return;
        source << "(stream, " << dest << ", callback);" << endl;
    }
}
