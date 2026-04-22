/*
        revis/core/src/object.c

        author: tracehillonsign
*/

#include "object.h"

#include "common.h"
#include "file.h"
#include "pack.h"
#include "sha256.h"

/* Это надо подумать куда перенести*/
int cmp_struct_by_name(const void *a, const void *b) {
  const struct Object *obj1 = (const struct Object *)a;
  const struct Object *obj2 = (const struct Object *)b;
  return strcmp(obj1->name, obj2->name);
}

// Функция создания объекта файла .revis/objects.
int write_blob(const char *path, char output_hash[HASH_LENGTH]) {
  size_t file_size = 0;
  if (get_file_content_size(path, &file_size) != 0) {
    fprintf(stderr, "Ошибка при получении количество байт файла (object.c : write_blob)\n");

    return 1;
  }

  // Получаем содержимое файла и хеш содержимого.
  char *content = NULL;
  if (get_file_content(path, file_size, &content) != 0) {
    fprintf(stderr, "Ошибка чтения содержимого файла (object.c : write_blob)\n");

    return 1;
  }
  char hash[HASH_LENGTH];

  if (get_hash(content, hash) == -1) {
    fprintf(stderr, "Ошибка вычисления хеша (object.c : write_blob)\n");
    free(content);

    return 1;
  }

  // Сжимаем содержимое файла.
  size_t compressed_len;
  unsigned char *compressed = pack(content, file_size, &compressed_len);

  if (compressed == NULL) {
    fprintf(stderr, "Получено NULL из pack (object.c : write_blob)\n");
    free(content);

    return 1;
  }

  free(content);

  // Собираем путь для записи файла.
  char full_path[MAX_PATH_LENGHT];
  snprintf(full_path, sizeof(full_path), ".revis/objects/%s", hash);

  FILE *blob_file = fopen(full_path, "wbx");

  if (blob_file == NULL) {
    if (errno == EEXIST) {
      free(compressed);
      strcpy(output_hash, hash);

      return 0;
    } else {
      perror("Ошибка создание объектного файла (object.c : write_blob)");
      free(compressed);

      return 1;
    }
  }

  /*
    Для записи размера используем 64-битный беззнаковый тип что бы точно
      знать сколько размер занимает места в файле.

    Потом делаем просто:
      uint64_t saved_size;
      fread(&saved_size, sizeof(uint64_t), 1, blob_file);
  */
  uint64_t size_to_write = file_size;
  if (fwrite(&size_to_write, sizeof(uint64_t), 1, blob_file) != 1) {
    if (ferror(blob_file)) {
      perror("Ошибка записи размера файла (object.c : write_blob)");
      free(compressed);
      fclose(blob_file);

      return 1;
    } else {
      fprintf(stderr,
              "Записано меньше байт размера чем ожидалось (object.c : "
              "write_blob)\n");
      free(compressed);
      fclose(blob_file);

      return 1;
    }
  }

  // Записываем сжатые данные в объектный файл.
  if (fwrite(compressed, 1, compressed_len, blob_file) != compressed_len) {
    if (ferror(blob_file)) {
      perror("Ошибка записи в файл (object.c : write_blob)");
      free(compressed);
      fclose(blob_file);

      return 1;
    } else {
      fprintf(stderr, "Записано меньше байт чем ожидалось (object.c : write_blob)\n");
      free(compressed);
      fclose(blob_file);

      return 1;
    }
  }

  free(compressed);
  fclose(blob_file);

  strcpy(output_hash, hash);

  return 0;
}

// Функция создания дерева объектов .revis/objects.
int write_tree(const char *path, char output_hash[HASH_LENGTH]) {
  printf("DEBUG write_tree: path='%s'\n", path);
  struct Object *objects = NULL;
  size_t object_count = 0;

  // Получаем структуру которая содержит все файлы и их данные.
  if (get_dir_tree(path, path, &objects, &object_count) != 0) {
    fprintf(stderr, "Не удалось получить древо объектов (object.c : write_tree)\n");

    return 1;
  }

  /* 
    Сортирует листья по имени для того
      что бы в любоый ОС у деревьев был одинаковый хеш.
  */
  qsort(objects, object_count, sizeof(struct Object), cmp_struct_by_name);

  char *content = NULL;
  size_t size = 0;

  // Открываем поток ввода-вывода для сборки итогового содержимого файла.
  FILE *stream = open_memstream(&content, &size);
  if (stream == NULL) {
    perror("Не удалось открыть поток записи (write_tree : object.c)");

    for (size_t i = 0; i < object_count; i++) {
      free(objects[i].name);
    }

    free(objects);

    return 1;
  }

  // Записываем данные в поток.
  for (size_t i = 0; i < object_count; i++) {
    fwrite(objects[i].type, 1, strlen(objects[i].type) + 1, stream);
    fwrite(objects[i].name, 1, strlen(objects[i].name) + 1, stream);
    fwrite(objects[i].hash, 1, strlen(objects[i].hash) + 1, stream);

    free(objects[i].name);
  }

  free(objects);
  fclose(stream);

  // Получаем хеш итогового содержимого который записывали в потоке.
  char hash[HASH_LENGTH];
  if (get_hash(content, hash) != 0) {
    fprintf(stderr, "Не удалось получить хеш (write_tree : object.c)\n");
    free(content);

    return 1;
  }

  char full_path[MAX_PATH_LENGHT];
  snprintf(full_path, sizeof(full_path), ".revis/objects/%s", hash);

  // Создаем и записываем данные в объект дерева .revis/objects.
  FILE *file = fopen(full_path, "wbx");
  if (file == NULL) {
    if (errno == EEXIST) {
      free(content);

      return 0;
    } else {
      perror("Ошибка создания файла дерева (write_tree : object.c)");
      free(content);

      return 1;
    }
  }

  if (fwrite(content, 1, size, file) != size) {
    fprintf(stderr, "Записано меньше байт чем ожидалось (write_tree : object.c)\n");
    free(content);

    return 1;
  }

  fclose(file);
  free(content);
  strcpy(output_hash, hash);

  return 0;
}

int write_commit(const char *message) {
  char tree_hash[HASH_LENGTH];
  if (write_tree(".", tree_hash) != 0) {
    fprintf(stderr, "Ошибка создания коммита (write_commit : object.c)\n");

    return 1;
  }

  char tree_path[MAX_PATH_LENGHT];
  snprintf(tree_path, sizeof(tree_path), ".revis/objects/%s", tree_hash);

  /*
    TODO:
      добавить парсинг .revis/CONFIG
  */

  const char *author = "enter_your_name";
  const char *email = "enter_your_email";
  char *content = NULL;
  size_t size = 0;

  // Открываем поток ввода-вывода для сборки итогового содержимого файла.
  FILE *stream = open_memstream(&content, &size);

  if (stream == NULL) {
    perror("Ошибка открытия потока (write_commit : object.c)");

    return 1;
  }

  // Записываем данные в поток.
  fwrite(author, 1, strlen(author) + 1, stream);
  fwrite(email, 1, strlen(email) + 1, stream);
  fwrite(message, 1, strlen(message) + 1, stream);
  fwrite(tree_hash, 1, strlen(tree_hash) + 1, stream);

  char commit_hash[HASH_LENGTH];
  fclose(stream);

  // Получаем хеш итогового содержимого коммита.
  if (get_hash(content, commit_hash) != 0) {
    fprintf(stderr, "Ошибка получения хеша содержимого коммита (write_commit : object.c)\n");
    free(content);

    return 1;
  }

  char commit_path[MAX_PATH_LENGHT];
  snprintf(commit_path, sizeof(commit_path), ".revis/objects/%s", commit_hash);

  // Создаем объектные файл коммита в .revis/objects.
  FILE *file_commit = fopen(commit_path, "wb");

  if (file_commit == NULL) {
    perror("Ошибка создания объектного файла коммита (write_commit : object.c)");
    free(content);

    return 1;
  }

  if (fwrite(content, 1, size, file_commit) != size) {
    fprintf(stderr, "Ошибка записи содержимого объектного файла комммта (write_commit : object.c)\n");
    free(content);
    fclose(file_commit);

    return 1;
  }

  free(content);
  fclose(file_commit);

  // Записываем хеш коммитва в .revis/HEAD.
  FILE *head_file = fopen(".revis/HEAD", "wb");

  if (head_file == NULL) {
    perror("Ошибка открытия HEAD файла для записи хеша (write_commit : object.c)");

    return 1;
  }

  if (fwrite(commit_hash, 1, strlen(commit_hash), head_file) != strlen(commit_hash)) {
    perror("Ошибка записи в HEAD");
    fclose(head_file);

    return 1;
  }

  fclose(head_file);

  return 0;
}