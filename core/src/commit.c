/*
        revis/core/src/commit.c

        author: tracehillonsign
*/

#include "commit.h"

#include "common.h"
#include "object.h"

// Функция создания нового коммита
int commit(const char *message) {
  if (write_commit(message) != 0) {
    fprintf(stderr, "Ошибка при создании коммита (commit : commit.c)\n");
  } else {
    printf("Коммит успешно создан!\n");
  }

  return 0;
}