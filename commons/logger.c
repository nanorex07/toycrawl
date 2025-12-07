#include "logger.h"

#define COLOR_RESET   "\033[0m"
#define COLOR_RED     "\033[31m"
#define COLOR_GREEN   "\033[32m"
#define COLOR_YELLOW  "\033[33m"

void log_info(char *message) {
    printf("%s[INFO] | %s%s\n", COLOR_GREEN, message, COLOR_RESET);
}

void log_warning(char *message) {
    printf("%s[WARNING] | %s%s\n", COLOR_YELLOW, message, COLOR_RESET);
}

void log_error(char *message) {
    printf("%s[ERROR] | %s%s\n", COLOR_RED, message, COLOR_RESET);
}

