/*
        revis/src/file.c

        author: tracehillonsign
*/

#include "common.h"

/* Открытие файла для записи/чтения. */
FILE *fopen_or_exit(const char *path, const char *mode)
{
    FILE *file = fopen(path, mode);

    if (file == NULL) {
        perror("[file:fopen_or_exit] Ошибка открытия файла");
        exit(1);
    }

    return file;
}

/* Создает файл. */
int create_file(const char *path)
{
    FILE *file = fopen_or_exit(path, "w");
    fclose(file);
    return 0;
}