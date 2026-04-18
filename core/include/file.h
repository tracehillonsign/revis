/*
	revis/core/build/file.h

	author: tracehillonsign
*/

#pragma once

#include "common.h"

int get_file_content_size(const char *path, size_t *file_size);
int get_file_content(const char *path, size_t file_size, char **content);