/*
	revis/core/src/commit.c

	author: tracehillonsign
*/

#include "common.h"
#include "object.h"

// Функция создания нового коммита
int commit(const char *message, const char *path) {
	(void)message;

	write_tree(path);

	return 0;
}