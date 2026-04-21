/*
        revis/core/src/file.c

        author: tracehillonsign
*/

#include "common.h"
#include "object.h"

// Вычисляет размер содержимого файла.
int get_file_content_size(const char *path, size_t *file_size) {
  FILE *file = fopen(path, "rb");

  if (file == NULL) {
    perror("Ошибка открытия файла (file.c : get_file_content_size)");
    return -1;
  }

  // Ставим указатель в конец файла и считываем значение.
  fseek(file, 0, SEEK_END);
  *file_size = ftell(file);

  fclose(file);

  return 0;
}

// Считывает содержимое файла. Отчистить content.
int get_file_content(const char *path, size_t file_size, char **content) {
  FILE *file = fopen(path, "rb");

  if (file == NULL) {
    perror("Ошибка чтения файла (file.c : get_file_content)");
    return 1;
  }

  // Выделяем память под содержимое файла.
  *content = (char *)malloc(file_size + 1);  // +1 для '\0'

  if (*content == NULL) {
    perror("Ошибка выделения памяти (file.c : get_file_content)");
    fclose(file);
    return 1;
  }

  /*
          TODO: Добавить проверку количества считанных байтов в fread.
  */

  // Читаем по 1 символу до конца файла.
  fread(*content, 1, file_size, file);
  (*content)[file_size] = '\0';

  fclose(file);

  return 0;
}

// Собирает рекурсивано массив файлов из директории.
int get_dir_tree(const char *root, const char *current, struct Object **objects, size_t *object_count) {
  DIR *dir = opendir(current);

  if (dir == NULL) {
    perror("Ошибка открытия директории (file.c : get_dir_tree)");
    return 1;
  }

  struct dirent *entry;
  struct stat st;

  // Рекурсивный обход директории.
  while ((entry = readdir(dir)) != NULL) {
    /*
            Пропускаем текущую и родительскую директорию для избегания
       бесконечного цикла.

            TODO:
                    Добавить проверку на нахождение файла в .revisignore
    */
    if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
      continue;
    }

    if (strcmp(entry->d_name, ".git") == 0 || strcmp(entry->d_name, ".revis") == 0) {
      continue;
    }

    char full_path[MAX_PATH_LENGHT];
    snprintf(full_path, MAX_PATH_LENGHT, "%s/%s", current, entry->d_name);

    const char *rel_path = full_path + strlen(root);
    if (*rel_path == '/') {
      rel_path++;
    }

    if (stat(full_path, &st) == 0) {
      if (S_ISDIR(st.st_mode)) {
        // Если путь являеться директорией вызываем в рекурсию get_dir_tree.
        if (get_dir_tree(root, full_path, objects, object_count) != 0) {
          fprintf(stderr, "Не удалось собрать дерево директории (file.c : get_dir_thee)\n");
          closedir(dir);
          return 1;
        }
      } else {
        // Выделяем память на +1 объект в objcets.
        *objects = (struct Object *)realloc(*objects, (*object_count + 1) * sizeof(struct Object));
        if (*objects == NULL) {
          perror("Ошибка выделения памяти (file.c : get_dir_tree)");

          free(*objects);
          (*object_count) = 0;
          *objects = NULL;
          closedir(dir);
          return 1;
        }

        // Создаем blob объект и получаем его хеш.
        char hash[HASH_LENGTH];
        if (write_blob(full_path, hash) == 1) {
          fprintf(stderr, "Не удалось создать blob объект (file.c : get_dir_tree)\n");

          free(*objects);
          (*object_count) = 0;
          *objects = NULL;
          closedir(dir);
          return 1;
        }

        // Создаем новый объект в objects и заполняем данными.
        struct Object *new_object = &(*objects)[*object_count];

        strcpy(new_object->type, "blob");

        new_object->name = strdup(rel_path);
        if (new_object->name == NULL) {
          perror(
              "Ошибка выделения памяти для поля названия (file.c : "
              "get_dir_tree)");

          free(*objects);
          (*object_count) = 0;
          *objects = NULL;
          closedir(dir);
          return 1;
        }

        strcpy(new_object->hash, hash);

        (*object_count)++;
      }
    } else {
      fprintf(stderr, "Cannot stat %s\n", full_path);
    }
  }

  closedir(dir);

  return 0;
}