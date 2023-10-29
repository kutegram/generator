#ifndef METHOD_H
#define METHOD_H

#include "schema.h"

#include <QTextStream>

void writeMethod(QTextStream& header, QTextStream& source, SCHEMA& schema, QString prefix, CONSTRUCTOR m, bool forward);

#endif // METHOD_H
