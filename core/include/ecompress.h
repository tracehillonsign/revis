/*
	revis/core/include/ecompress.h

	author: tracehillnosign
*/

#ifndef ECOMPRESS_H
#define ECOMPRESS_H

#include "common.h"

unsigned char *ecompress(char *content, size_t content_len, size_t *dest_len);

#endif