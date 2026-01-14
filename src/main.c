// Crawler Main.c

#include "frontier.h"
#include "html_parser.h"
#include "logger.h"
#include "memtable.h"
#include "net.h"
#include "xxhash.h"
#include <inttypes.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

const size_t max_bytes_to_process = 300;

int main(int argc, char *argv[]) {
    log_info("Welcome to ToyCrawl");

    // html_parser *parser = initialize_parser(max_bytes_to_process);

    // net_client *client = make_net_client();

    // FILE* output_file = fopen("../test_output/index.html", "wb");

    // do_get_request(client, output_file,
    // "https://en.wikipedia.org/wiki/Rocket_(disambiguation)");
    // parse_file("../test_output", parser);

    // free_net_client(client);
    // free_parser(parser);

    // frontier *frontier = initialize_frontier("../test_output");

    // for (int i = 1; i < argc; i++) {
    //     uint64_t hash = XXH64(argv[i], strlen(argv[i]), 0);
    //     frontier_add(frontier, 1, hash, argv[i]);
    // }

    // frontier_item *item;
    // do {
    //     item = frontier_pop(frontier);
    //     if (item) {
    //         log_frontier_item(item);
    //         free_frontier_item(item);
    //     }
    // } while (item != NULL);

    // free_frontier(frontier);

    return 0;
}
