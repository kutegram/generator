#ifndef SHARED_H
#define SHARED_H

#include "schema.h"

#include <QTextStream>

QString prepareName(QString prefix, QString raw);
void writeVector(QTextStream& source, PARAM p, QString prefix);
void writeParam(QTextStream& source, PARAM p, QString prefix);
void readVector(QTextStream& source, PARAM p, QString prefix);
void readParam(QTextStream& source, PARAM p, QString prefix);

#endif // SHARED_H
