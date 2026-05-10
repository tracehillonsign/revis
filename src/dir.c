/*
        revis/src/dir.c

        author: tracehillonsign
*/

#include "common.h"

/* Проверяет существует ли директория. */
int dir_exits(const char *path)
{
    DIR *dir = opendir(path);

    if (dir != NULL) {
        closedir(dir);
        return 1;
    }

    return 0;
}