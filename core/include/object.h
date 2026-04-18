/*
	revis/core/include/object.h

	author: tracehillonsign
*/

#pragma once

#define HASH_LENGTH 65
#define MAX_PATH_LENGHT 512

int write_blob(const char *path, char output_hash[HASH_LENGTH], size_t *file_size);
int write_tree(const char *path, char output_hash[HASH_LENGTH]);