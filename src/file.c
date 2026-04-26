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
        return 1;
    }
}

/* Записывает данные в файл. */
int wfile(const char *path, const void *data, size_t size)
{
    FILE *file = fopen(path, "wb");

    if (file == NULL) {
        perror("wfile: fopen");
        return -1;
    }

    if (fwrite(data, 1, size, file) != size) {
        perror("wfile: fwrite");
        fclose(file);
        return -1;
    }

    fclose(file);
    return 0;
}

/* Создает файл. */
int cfile(const char *path)
{
    FILE* file = fopen(path, "w");

    if (file == NULL){
        perror("не удалось создать файл (cfile : file.c)");
        return -1;
    }

    fclose(file);
    return 0;
}