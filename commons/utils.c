#include <stdio.h>
#include "logger.h"

FILE* open_file_with_error(const char *filename) {
    FILE* fp = fopen(filename, "rb");

    if (fp == NULL) {
        char buff[100];
        sprintf(buff, "unable to open file, %s", filename);
        log_error(buff);
    }
    return fp;
}