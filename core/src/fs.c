/*
	revis/core/src/fs.c

	author: tracehillonsign
*/

#include "common.h"

// Создает файл.
int create_file(const char *file_name){
	FILE *file;
	file = fopen(file_name, "w");

	if (file == NULL) {
		perror("Error(create_file)");
		return 1;
	}

	fclose(file);

	return 0;
}

// Создает директорию.
int create_dir(const char *path_name) {
	if (mkdir(path_name, 0755) == -1) {
		perror("Error(create_dir)");
		return 1;
	}

	return 0;
}

// Возвращает размер содержимого файла.
size_t get_file_size(const char *file_path) {
	FILE *file;
	file = fopen(file_path, "rb");

	if (file == NULL) {
		perror("Error(get_file_size)");
		return -2;
	}

	// Ставим курсор в конец, считываем кол-во, ставим обратно.
	fseek(file, 0, SEEK_END);
	size_t len = ftell(file);
	fseek(file, 0, SEEK_SET);

	fclose(file);

	return len;
}

// Рекурсивно обходит директорию вызывая функцию callback для каждого файла.
int walk(const char *path, char *(*callback)(const char *path)) {
	DIR *dir = opendir(path);

	if (dir == NULL) {
		return 1;
	}

	char full_path[512];

	struct dirent *entry;
	struct stat st;

	// Перебираем все элементы в директории.
	while ((entry = readdir(dir)) != NULL) {
		// Пропускаем текущую и родительскую директорию для избежания бесконечного цикла.
		if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
			continue;
		}

		if (strcmp(entry->d_name, ".revis") == 0 || strcmp(entry->d_name, ".git") == 0) {
			continue;
		}

		// Собираем путь к файлу/директории
		snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);

		// Проверяем являеться ли полученный путь директорией или файлом.
		if (stat(full_path, &st) == 0) {
			if (S_ISDIR(st.st_mode)) {
				walk(full_path, callback);
			} else {
				callback(full_path);
			}
		} else {
			fprintf(stderr, "Cannot stat %s\n", full_path);
		}
	}

	return 0;
}