/*
        revis/core/src/sha256.h

        author: tracehillonsign
*/

#include "common.h"

/*
        Изначально код этого модуля написан под OpenSSL 1.1.0 (вроде эта
   версия). Код этого модуля на данный момент скопирован из какого-то источника.

    Его надо разобрать/понять и возможно переписать.
*/

// Функция вычисления хеша строки.
int get_hash(const char *string, char outputBuffer[65]) {
  unsigned char hash[SHA256_DIGEST_LENGTH];
  EVP_MD_CTX *mdctx = EVP_MD_CTX_new();
  if (!mdctx) {
    fprintf(stderr, "EVP_MD_CTX_new failed\n");
    return -1;
  }

  if (EVP_DigestInit_ex(mdctx, EVP_sha256(), NULL) != 1 ||
      EVP_DigestUpdate(mdctx, string, strlen(string)) != 1 ||
      EVP_DigestFinal_ex(mdctx, hash, NULL) != 1) {
    fprintf(stderr, "EVP digest failed\n");
    EVP_MD_CTX_free(mdctx);
    return -1;
  }

  EVP_MD_CTX_free(mdctx);

  for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
    sprintf(outputBuffer + (i * 2), "%02x", hash[i]);
  }
  outputBuffer[64] = '\0';

  return 0;
}