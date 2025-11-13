#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <stdint.h>

#include "util.h"

#define KEY_FILE "key"
#define TIME_STEP 30

int main() {
    uint8_t *key;
    int key_len;
    if ((key_len = read_key(&key, KEY_FILE)) < 0) {
        return 1;
    }

    time_t current_time;
    long counter;
    char totp[7];

    // Clear screen and print the box structure once
    system("clear");
    // Hide cursor
    printf("\033[?25l");
    printf("┌──────────────────────────────────┐\n");
    printf("│          TOTP Generator          │\n");
    printf("├──────────────────────────────────┤\n");
    printf("│                                  │\n");
    printf("│   Current Code: ......           │\n");
    printf("│                                  │\n");
    printf("└──────────────────────────────────┘\n");
    
    while (1) {
        current_time = time(NULL);
        counter = current_time / TIME_STEP;
        long time_left = TIME_STEP - (current_time % TIME_STEP);

        generate_totp(key, key_len, counter, totp);

        printf("\033[5;19H%s", totp);
        fflush(stdout);
        
        for (long t = time_left; t > 0; t--) {
            printf("\033[8;1H\rCode refreshes in %2ld seconds... ", t);
            fflush(stdout);
            sleep(1);
        }
        printf("\033[8;1H\rCode refreshing now...            ");
        fflush(stdout);
    }

    free(key);
    return 0;
}
