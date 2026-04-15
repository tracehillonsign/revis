/*
	revis/core/src/ecompress.c

	author: tracehillonsign
*/

#include "common.h"

// Сжимает полученную строку через zlib.
unsigned char *ecompress(char *content, size_t content_len, size_t *dest_len) {
	// Получаем максимально возможный размер сжатых данных для исходного блока.
	uLongf compressed_size = compressBound(content_len);
	unsigned char *compressed = (unsigned char *)malloc(compressed_size);

	if (compressed == NULL) {
		perror("Error(ecompress)");
		return NULL;
	}

	// Сжимаем полученный content.
	int result = compress(compressed, &compressed_size, content, content_len);
	if (result != Z_OK) {
		free(compressed);
		return NULL;
	}

	free(content);

	// Записываем длину сажтого блока.
	*dest_len = compressed_size;
	return compressed;
}