/*
	revis/core/src/object.c

	author: tracehillonsign
*/ 

#include "common.h"
#include "fs.h"
#include "ecompress.h"
#include "sha256.h"

/*
	char *create_blob(const char *file_path, size_t file_size)
*/

// Создает объект в .revis/objects/
char *create_blob(const char *file_path) {
	FILE *file;
	file = fopen(file_path, "rb");

	if (file == NULL) {
		perror("Error(create_blob)");
		return NULL;
	}

	size_t file_size;
	file_size = get_file_size(file_path);
	char *content = (char *)malloc(file_size + 1);
	if (content) {
		fread(content, 1, file_size, file);
		content[file_size] = '\0';
	}

	char *hash = (char *)malloc(65);
	sha256_easy_hash_hex(content, file_size, hash);
	hash[64] = '\0';

	fclose(file);

	size_t compressed_len;
	unsigned char *compressed = ecompress((unsigned char *)content, file_size, &compressed_len);

	char full_path[512];
	snprintf(full_path, sizeof(full_path), ".revis/objects/%s", hash);

	printf("%s", full_path);
	printf("\n");

	FILE *blob_file;
	blob_file = fopen(full_path, "wb");

	if (blob_file) {
    // Файл уже существует — ничего не делаем (или возвращаем ошибку)
    fclose(blob_file);
    free(compressed);
    return NULL; // или просто пропускаем создание
}

	if (!blob_file) {
		perror("fopen filed");
		return NULL;
	}

	fwrite(compressed, 1, compressed_len, blob_file);
	fclose(blob_file);

	free(compressed);

	return hash;
}