/*
	revis/core/src/main.c

	author: tracehillonsign
*/

#include "common.h"
#include "init.h"
#include "commit.h"

// Точка входа в программу.
int main(int argc, char *argv[]) {
	if (argc < 2) {
		return 1;
	}

	if (strcmp(argv[1], "init") == 0) {
		init();
	}

	if (strcmp(argv[1], "commit") == 0) {
		commit(argv[2]);
	}

	return 0;
}