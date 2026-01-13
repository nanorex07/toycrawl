#ifndef HTML_PARSER_H
#define HTML_PARSER_H

#include "utils.h"
#include <libxml/HTMLparser.h>
#include <libxml/HTMLtree.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <logger.h>
#include <stddef.h>
#include <stdlib.h>
#include <strings.h>

typedef struct {
    size_t max_bytes;
    FILE *content_file;
    FILE *urls_file;
    char *content_buffer;
} html_parser;

html_parser *initialize_parser(const size_t max_bytes);
void free_parser(html_parser *parser);

void parse_file(char *folder, html_parser *parser);

#endif
