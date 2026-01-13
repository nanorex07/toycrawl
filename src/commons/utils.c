#include "utils.h"
#include <stdio.h>

FILE* open_file_with_error(const char *filename, char *mode) {
    FILE* fp = fopen(filename, mode);

    if (fp == NULL) {
        log_error("unable to open file, %s", filename);
    }
    return fp;
}

FILE* open_create_file(const char *filename, char *mode) {
    FILE* fp = fopen(filename, mode);
    if (fp!=NULL) return fp;

    fp = fopen(filename, "w");
    if (fp == NULL) {
        log_error("unable to open file, %s", filename);
        return NULL;
    }
    fclose(fp);
    fp = fopen(filename, mode);
    if (fp == NULL) {
        log_error("unable to open file, %s", filename);
        return NULL;
    }
    return fp;
}

int string_starts_with(const char* str, const char* prefix) {
    if (strlen(str) < strlen(prefix)) {
        return 0;
    }
    return strncmp(str, prefix, strlen(prefix)) == 0;
}

void write_uintN_le(uint8_t *buf, size_t x, int n) {
    for (int i = 0; i < n; i++)
        buf[i] = (x >> (8 * i)) & 0xFF;
}

size_t read_uintN_le(const uint8_t *buf, int n) {
    size_t x = 0;
    for (int i = n-1; i >= 0; i--)
        x = (x << 8) | buf[i];
    return x;
}
