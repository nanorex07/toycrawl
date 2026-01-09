#include "html_parser.h"
#include "utils.h"

html_parser *initialize_parser(const size_t max_bytes) {
    html_parser *parser = malloc(sizeof(html_parser));
    parser->max_bytes = max_bytes;
    parser->content_file = NULL;
    parser->urls_file = NULL;
    parser->content_buffer = malloc(max_bytes);
    return parser;
}

void free_parser(html_parser *parser) {
    fclose(parser->content_file);
    fclose(parser->urls_file);
    free(parser->content_buffer);
    free(parser);
}

int include_url(const char *url) {
    size_t size = strlen(url);
    if (!size)
        return 0;
    return string_starts_with(url, "/") || string_starts_with(url, "https://");
}

void walk(xmlNode *node, html_parser *parser) {
    for (xmlNode *cur = node; cur; cur = cur->next) {
        if (cur->type == XML_ELEMENT_NODE) {
            if (xmlStrcasecmp(cur->name, (const xmlChar *)"a") == 0) {
                xmlChar *href = xmlGetProp(cur, (const xmlChar *)"href");
                if (href && include_url((const char *)href)) {
                    fputs((const char *)href, parser->urls_file);
                    fputc('\n', parser->urls_file);
                    xmlFree(href);
                }
            }
        } else if (cur->type == XML_TEXT_NODE) {
            xmlChar *content = xmlNodeGetContent(cur);
            if (content && xmlStrlen(content) > 0) {
                fputs((const char *)content, parser->content_file);
            }
            xmlFree(content);
        }
        walk(cur->children, parser);
    }
}

void parse_file(char *folder, html_parser *parser) {

    int filename_len = MAX_FILE_NAME_LENGTH + strlen(folder) + 1;
    char html_filename[filename_len], urls_filename[filename_len],
        content_filename[filename_len];

    snprintf(html_filename, filename_len, "%s/%s", folder, HTML_FILE_NAME);
    snprintf(urls_filename, filename_len, "%s/%s", folder, URLS_FILE_NAME);
    snprintf(content_filename, filename_len, "%s/%s", folder,
             CONTENT_FILE_NAME);

    parser->content_file = open_file_with_error(content_filename, "w+");
    parser->urls_file = open_file_with_error(urls_filename, "w+");

    htmlDocPtr doc = htmlReadFile(html_filename, NULL,
                                  HTML_PARSE_NOWARNING | HTML_PARSE_NOERROR);
    if (!doc) {
        log_error("Failed to load html file");
        return;
    }

    walk(xmlDocGetRootElement(doc), parser);

    xmlFreeDoc(doc);
    xmlCleanupParser();
}
