/*
        revis/src/main.c

        author: tracehillonsign
*/

#include "common.h"
#include "init.h"

int main(int argc, char *argv[])
{
    /*
            TODO:
            Реализовать динамическую обработку аргументов.
    */

    if (argc < 2) {
        return 1;
    }

    if (strcmp(argv[1], "init") == 0) {
        init();
    }
}