/*
        revis/src/dir.c

        author: tracehillonsign
*/

#include "common.h"

/* Проверяет существует ли директория. */
int dir_exists(const char *path)
{
    DIR *dir = opendir(path);

    if (dir != NULL) {
        closedir(dir);
        return 1;
    }

    return 0;
}

/* Создает директорию. */
int create_dir(const char *path)
{
    /*
            TODO: Проврка корректности создания директории
    */
    return mkdir(path, 0755);
}