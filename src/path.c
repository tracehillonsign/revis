/*
        revis/src/path.c

        author: tracehillonsign
*/

#include "common.h"

/* Проверяет существование пути. */
int pexist(const char *path)
{
    struct stat st;

    if (stat(path, &st) == 0) {
        return 1;
    } else {
        return 0;
    }
}