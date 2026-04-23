/*
        revis/src/file.c

        author: tracehillonsign
*/

#include "common.h"

/* Проверяет является ли путь файлом. */
int ifile(const char *path)
{
    struct stat st;

    if (stat(path, &st) == -1) {
        perror("ошибка stat(ifile : file)");
        return -1;
    }

    if (S_ISREG(st.st_mode)) {
        return 0;
    } else {
        return -2;
    }
}