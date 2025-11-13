#include "util.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/hmac.h>

int b32_decode(const char *b32, uint8_t **output) {
    int length = 0;
    int buffer = 0;
    int bits_left = 0;
    int count = 0;
    *output = malloc(strlen(b32) * 5 / 8 + 1);
    if (!*output) {
        return -1;
    }

    for (const char *ptr = b32; *ptr; ++ptr) {
        char c = *ptr;
        if (c >= 'A' && c <= 'Z') {
            c -= 'A';
        } else if (c >= '2' && c <= '7') {
            c -= '2' - 26;
        } else {
            continue;
        }

        buffer = (buffer << 5) | c;
        bits_left += 5;
        if (bits_left >= 8) {
            (*output)[length++] = buffer >> (bits_left - 8);
            bits_left -= 8;
        }
    }
    return length;
}


int read_key(uint8_t **key, const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        perror("Error opening key file");
        return -1;
    }

    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    char *b32_key = malloc(size + 1);
    if (b32_key == NULL) {
        fclose(fp);
        perror("Failed to allocate memory for key");
        return -1;
    }

    fread(b32_key, 1, size, fp);
    b32_key[size] = '\0';
    fclose(fp);

    // Remove newline character if present
    char *newline = strchr(b32_key, '\n');
    if (newline) {
        *newline = '\0';
    }

    int key_len = b32_decode(b32_key, key);
    free(b32_key);

    if (key_len <= 0) {
        fprintf(stderr, "Failed to decode base32 key\n");
        free(*key);
        return -1;
    }

    return key_len;
}

void generate_totp(const uint8_t *key, size_t key_len, long counter, char *totp) {
    unsigned char counter_bytes[8];
    for (int i = 7; i >= 0; i--) {
        counter_bytes[i] = counter & 0xff;
        counter >>= 8;
    }

    unsigned char *hmac_result;
    unsigned int hmac_len;

    hmac_result = HMAC(EVP_sha1(), key, key_len, counter_bytes, 8, NULL, &hmac_len);

    int offset = hmac_result[hmac_len - 1] & 0x0f;
    long truncated_hash =
        ((hmac_result[offset] & 0x7f) << 24) |
        ((hmac_result[offset + 1] & 0xff) << 16) |
        ((hmac_result[offset + 2] & 0xff) << 8) |
        (hmac_result[offset + 3] & 0xff);

    long otp = truncated_hash % 1000000;

    sprintf(totp, "%06ld", otp);
}
