/*
        revis/include/file.h

        author: tracehillonsign
*/

#pragma once

#include <stddef.h>

int ifile(const char *path);
int wfile(const char *path, const void *data, size_t size);
int cfile(const char *path);