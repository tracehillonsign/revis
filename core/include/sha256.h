/*
        revis/core/include/sha256.h

        author: tracehillonsign
*/

#pragma once

int get_hash(const char *string, char outputBuffer[65]);
int get_hash_data(const unsigned char *data, size_t len, char outputBuffer[65]);