#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>
#include "util.h"

#define DB_FILE "database"
#define TIME_STEP 30
#define MAX_USERS 100
#define MAX_LINE_LEN 256

typedef struct {
    char username[50];
    uint8_t *key;
    int key_len;
} User;

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <totp>\n", argv[0]);
        return 1;
    }

    char *user_totp = argv[1];

    FILE *fp = fopen(DB_FILE, "r");
    if (fp == NULL) {
        perror("Error opening database file");
        return 1;
    }

    User users[MAX_USERS];
    int user_count = 0;
    char line[MAX_LINE_LEN];

    while (fgets(line, sizeof(line), fp) && user_count < MAX_USERS) {
        char *username = strtok(line, " \n");
        char *b32_key = strtok(NULL, " \n");

        if (username && b32_key) {
            strcpy(users[user_count].username, username);
            users[user_count].key_len = b32_decode(b32_key, &users[user_count].key);
            if (users[user_count].key_len <= 0) {
                fprintf(stderr, "Failed to decode base32 key for user %s\n", users[user_count].username);
                continue;
            }
            user_count++;
        }
    }
    fclose(fp);

    long current_time = time(NULL);
    long counter = current_time / TIME_STEP;

    char generated_totp[7];
    int verified = 0;
    char *verified_user = NULL;

    for (int i = 0; i < user_count; i++) {
        generate_totp(users[i].key, users[i].key_len, counter, generated_totp);
        if (strcmp(user_totp, generated_totp) == 0) {
            verified = 1;
            verified_user = users[i].username;
            break;
        }
    }

    if (verified) {
        printf("Successfully Verified for user: %s\n", verified_user);
    } else {
        printf("Verification Failed\n");
    }

    for (int i = 0; i < user_count; i++) {
        free(users[i].key);
    }

    return 0;
}
