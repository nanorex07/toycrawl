#ifndef HTML_PARSER_H
#define HTML_PARSER_H

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include "logger.h"
#include "charlist.h"

typedef struct {
    size_t max_bytes;
    // char* stack;
    // long stack_top;
} html_parser;

html_parser* initialize_parser(const size_t *max_bytes);
void free_parser(html_parser *parser);

void parse_file(char *filename, char* output_folder, html_parser* parser);

#endif
