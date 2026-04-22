/*
        revis/core/src/commit.c

        author: tracehillonsign
*/

#include "commit.h"

#include "common.h"
#include "object.h"

// Функция создания нового коммита
int commit(const char *message) {
  write_commit(message);

  return 0;
}