// Crawler Main.c

#include "logger.h"
#include "html_parser.h"
#include <stddef.h>

const size_t max_bytes_to_process = 100000;


int main(void) {
    log_info("Welcome to ToyCrawl");

    html_parser* parser = initialize_parser(&max_bytes_to_process);

    parse_file("../test.html", "", parser);
    free_parser(parser);
    return 0;
}

