#ifndef CONSTRUCTOR_H
#define CONSTRUCTOR_H

#include "schema.h"

#include <QTextStream>

void writeInterface(QTextStream& header, QTextStream& source, SCHEMA& schema, QString prefix, QString interface);

#endif // CONSTRUCTOR_H
