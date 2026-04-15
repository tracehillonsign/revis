/*
	revis/core/src/init.c

	author: tracehillonsign
*/ 

#include "common.h"
#include "fs.h"

// Функция инициализации репозитория.
int init() {
	create_dir(".revis");
	create_dir(".revis/objects");
	create_file(".revis/HEAD");

	return 0;
}