/*
        revis/src/dir.c

        author: tracehillonsign
*/

#include "common.h"

/* Проверяет является ли путь директорией. */
int idir(const char *path)
{
    struct stat st;

    if (stat(path, &st) == -1) {
        perror("ошибка stat(idir : file)");
        return -1;
    }

    if (S_ISDIR(st.st_mode)) {
        return 0;
    } else {
        return 1;
    }
}

/* Создает директорию. */
int cdir(const char *path)
{
    if (mkdir(path, 0755) == -1){
        perror("не удалось создать директорию");
        return -1;
    }

    return 0;
}