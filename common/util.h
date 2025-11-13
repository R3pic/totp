#ifndef UTIL_H
#define UTIL_H

#include <openssl/evp.h>
#include <openssl/hmac.h>
#include <stdint.h>
#include <stdlib.h>

int b32_decode(const char *b32, uint8_t **output);
int read_key(uint8_t **key, const char *filename);
void generate_totp(const uint8_t *key, size_t key_len, long counter, char *totp);

#endif // UTIL_H
