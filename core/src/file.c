/*
	revis/core/src/file.c

	author: tracehillonsign
*/

#include "common.h"

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
	*content = (char *)malloc(file_size + 1); // +1 для '\0'

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