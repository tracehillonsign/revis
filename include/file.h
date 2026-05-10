/*
        revis/include/file.h

        author: tracehillonsign
*/

#pragma once

#include "common.h"

FILE *fopen_or_exit(const char *path, const char *mode);
int create_file(const char *path);