/*
	revis/core/src/object.c

	author: tracehillonsign
*/

#include "common.h"
#include "sha256.h"
#include "file.h"
#include "pack.h"
#include "object.h"

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
		Для записи размера используем 64-битный беззнаковый тип что бы точно знать
		сколько размер занимает места в файле.

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
			fprintf(stderr, "Записано меньше байт размера чем ожидалось (object.c : write_blob)\n");
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
int write_tree(const char *path) {
	struct Object *objects = NULL;
	size_t object_count = 0;

	if (get_dir_tree(path, path, &objects, &object_count) != 0) {
		fprintf(stderr, "Не удалось получить древо объектов (object.c : write_tree)\n");

		return 1;
	}

	if (objects != NULL) {
		for (size_t i = 0; i < object_count; i++) {
			
		}
	}

	if (objects != NULL ) {
		for (size_t i = 0; i < object_count; i++) {
			printf("%s %s %s\n", objects[i].type, objects[i].name, objects[i].hash);
			free(objects[i].name);
		}

		free(objects);
	}

	return 0;
}