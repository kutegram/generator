#include "generator.h"

int main(int argc, char *argv[])
{
    generate(":/mtproto.json", "MT", 0, "tgstream.h");
    generate(":/telegram.json", "TL", 152, "tgstream.h");
    qDebug("Finished.");
}
