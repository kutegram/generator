#ifndef GENERATOR_H
#define GENERATOR_H

#include <QList>
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <json.h>

#include "schema.h"
#include "shared.h"
#include "constructor.h"
#include "method.h"

void writeEnum(QTextStream& header, SCHEMA& schema, QString prefix);
void generate(QString jsonPath, QString prefix, qint32 layer, QString streamHeaderPath);

#endif // GENERATOR_H
