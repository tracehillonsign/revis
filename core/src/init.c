/*
        revis/core/src/init.c

        author: tracehillonsign
*/

#include "common.h"

// Функция инициализации репозитория
int init() {
  // Создание рабочей директории .revis.
  if (mkdir(".revis", 0755) == -1) {
    return 1;
  }

  // Создание директории хранение объектов .revis/objects.
  if (mkdir(".revis/objects", 0755) == -1) {
    return 1;
  }

  // Создание заголовочного файла .revis/HEAD.
  FILE *head = fopen(".revis/HEAD", "w");

  if (head == NULL) {
    return 1;
  }

  // Создание файла конфигурации .revis/CONFIG.
  FILE *config = fopen(".revis/CONFIG", "w");

  if (config == NULL) {
    fclose(head);
    return 1;
  }

  fclose(head);
  fclose(config);

  return 0;
}