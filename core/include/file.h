/*
        revis/core/build/file.h

        author: tracehillonsign
*/

#pragma once

#include "common.h"
#include "object.h"

int get_file_content_size(const char *path, size_t *file_size);
int get_file_content(const char *path, size_t file_size, char **content);
int get_dir_tree(const char *root, const char *current, struct Object **objects,
                 size_t *object_count);