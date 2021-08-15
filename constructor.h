#ifndef CONSTRUCTOR_H
#define CONSTRUCTOR_H

#include "schema.h"

#include <QTextStream>

void writeInterface(QTextStream& header, QTextStream& source, SCHEMA& schema, QString prefix, QString interface, bool forward);

#endif // CONSTRUCTOR_H
