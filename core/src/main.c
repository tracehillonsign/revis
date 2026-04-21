/*
        revis/core/src/main.c

        author: tracehillonsign
*/

#include "commit.h"
#include "common.h"
#include "init.h"

// Точка входа в программу.
int main(int argc, char *argv[]) {
  if (argc < 2) {
    return 1;
  }

  // Функция инициализации репозитория.
  if (strcmp(argv[1], "init") == 0) {
    if (init() == 0) {
      fprintf(stdout, "Репозиторий инициализирован.\n");
    }
  }

  // Функция создания нового коммита.
  if (strcmp(argv[1], "commit") == 0) {
    commit(argv[2], ".");
  }

  return 0;
}