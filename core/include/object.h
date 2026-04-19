/*
	revis/core/include/object.h

	author: tracehillonsign
*/

#pragma once

#define HASH_LENGTH 65
#define MAX_PATH_LENGHT 512
#define MAX_TYPE_LENGHT 5 // tree(4) или blob(4) + '\0'(1)

struct Object {
	char type[MAX_TYPE_LENGHT];
	char *name;
	char hash[HASH_LENGTH];
};

int write_blob(const char *path, char output_hash[HASH_LENGTH]);
int write_tree(const char *path);