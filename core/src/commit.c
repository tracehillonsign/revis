/*
	revis/core/src/commit.c

	author: tracehillonsign
*/ 

#include "common.h"
#include "fs.h"
#include "object.h"

// Функция создания коммита | снимка репозитория.
int commit(const char *message) {
	walk(".", create_blob);

	return 0;
}