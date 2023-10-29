#include "generator.h"

int main(int argc, char *argv[])
{
    generate(":/mtproto.json", "MT", 0, "tgstream.h");
    generate(":/api.tl", "TL", 166, "tgstream.h");
    qDebug("Finished.");
}
