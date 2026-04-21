/*
        revis/core/src/pack.c

        author: tracehillonsign
*/

#include "common.h"

// Функция сжатия строки. После использовния отчистить compressed.
unsigned char *pack(char *content, size_t content_len, size_t *dest_len) {
  // Получаем максимально возможный размер сжатых данных.
  uLongf compressed_size = compressBound(content_len);
  unsigned char *compressed = (unsigned char *)malloc(compressed_size);

  if (compressed == NULL) {
    fprintf(stderr, "failed to allocate memory (pack.c : pack)\n");
    return NULL;
  }

  // Сжимаем полученный content.
  int result = compress(compressed, &compressed_size, (Bytef *)content, content_len);

  if (result != Z_OK) {
    fprintf(stderr, "failed to compress content (pack.c : pack)\n");
    free(compressed);
    return NULL;
  }

  // Записывает итоговую длину сжатых данных.
  *dest_len = compressed_size;

  return compressed;
}