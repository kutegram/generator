#include "generator.h"

int main(int argc, char *argv[])
{
    generate(":/mtproto.json", "MT", 0);
    generate(":/telegram.json", "TL", 145);
    qDebug("Finished.");
}
