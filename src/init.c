/*
        revis/src/init.c

        author: tracehillonsign
*/

#include "common.h"
#include "dir.h"
#include "file.h"

int init()
{

    /*
            TODO:
            Реализовать нормальный инициализатор репозитория.
    */

    if (dir_exists(".revis") == 1) {
        fprintf(stderr, "Репозиторий уже инициализирован.\n");
        return -1;
    }

    create_dir(".revis/");
    create_dir(".revis/objects");

    create_file(".revis/HEAD");
    create_file(".revis/config");
    return 0;
}