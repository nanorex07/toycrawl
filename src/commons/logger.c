#include "logger.h"

static void vlog_with_prefix(const char *color, const char *prefix, const char *fmt, va_list ap) {
    printf("%s%s | ", color, prefix);
    vprintf(fmt, ap);
    printf("%s\n", COLOR_RESET);
}

void log_info(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    vlog_with_prefix(COLOR_GREEN, "[INFO]", fmt, ap);
    va_end(ap);
}

void log_warning(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    vlog_with_prefix(COLOR_YELLOW, "[WARNING]", fmt, ap);
    va_end(ap);
}

void log_error(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    vlog_with_prefix(COLOR_RED, "[ERROR]", fmt, ap);
    va_end(ap);
}
