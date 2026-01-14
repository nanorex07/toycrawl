#ifndef UTILS_H
#define UTILS_H
#include "logger.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

// Common func macros
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

// Common var macros
#define HTML_FILE_NAME "index.html"
#define URLS_FILE_NAME "urls.txt"
#define CONTENT_FILE_NAME "content.txt"

#define MAX_FILE_NAME_LENGTH 200

FILE *open_file_with_error(const char *filename, char *mode);
FILE *open_create_file(const char *filename, char *mode);

int string_starts_with(const char *str, const char *prefix);

void write_uintN_le(uint8_t *buf, size_t x, int n);
size_t read_uintN_le(const uint8_t *buf, int n);

#endif
