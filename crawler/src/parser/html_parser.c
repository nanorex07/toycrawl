#include "html_parser.h"
#include "logger.h"
#include <lexbor/html/html.h>
#include <stddef.h>
#include <stdlib.h>

html_parser* initialize_parser(const size_t *max_bytes) {
    html_parser *parser = malloc(sizeof(html_parser));
    parser->max_bytes = *(size_t*)max_bytes;
    return parser;
}

void free_parser(html_parser *parser) {
    free(parser);
}

void parse_file(char *filename, char* output_folder, html_parser *parser) {
    FILE *fp = open_file_with_error(filename);
    
    lxb_status_t status;
    lxb_html_document_t *document = lxb_html_document_create();
    if (document == NULL) {
        log_error("failed to create html document.");
        exit(0);
    }

    status = lxb_html_document_parse_chunk_begin(document);
    if (status != LXB_STATUS_OK) {
        log_error("failed to begin chunk parsing.");
        exit(0);
    }

    size_t scanned = 0;
    while (scanned < parser->max_bytes) {
        char *batch = malloc(sizeof(char)*100);
        size_t read = fread(batch, 1, 100, fp);
        if (read <= 0) {
            break;
        }
        scanned += read;

        lxb_html_document_parse_chunk(document, batch, read);
       
    }

    fclose(fp);
}
