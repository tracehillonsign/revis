/*
        revis/include/file.h

        author: tracehillonsign
*/

#pragma once
#ifndef FILE_H
#define FILE_H

#include <stddef.h>

int ifile(const char *path);
int wfile(const char *path, const void *data, size_t size);
int cfile(const char *path);

#endif