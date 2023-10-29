#ifndef GENERATOR_H
#define GENERATOR_H

#include <QList>
#include <QFile>
#include <QStringList>

#include "schema.h"
#include "shared.h"
#include "constructor.h"
#include "method.h"

void writeEnum(QTextStream& header, SCHEMA& schema, QString prefix);
void generate(QString filePath, QString prefix, qint32 layer, QString streamHeaderPath);

#endif // GENERATOR_H
