#ifndef FRONTIER_H
#define FRONTIER_H

#include "utils.h"
#include "stdio.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define FRNTR_LOG_FILENAME "frontier.log"
#define FRNTR_OFFSET_FILENAME "frontier.offset"

#define FRNTR_DEPTH_BYTES 1
#define FRNTR_URLLEN_BYTES 2
#define FRNTR_URLHASH_BYTES 8
#define FRNTR_HEADER_LEN 11

typedef struct {
    uint8_t depth;
    uint32_t url_len;
    uint64_t url_hash;
    char *url;
} frontier_item;

typedef struct {
    FILE *frontier_log;
    FILE *frontier_offset;
    uint64_t read_head;
    frontier_item *peek_cache;
} frontier;

frontier *initialize_frontier(char *frontier_folder);
void free_frontier(frontier *frntr);

void free_frontier_item(frontier_item *item);
void log_frontier_item(frontier_item *item);

bool frontier_empty(frontier *frntr);
frontier_item *frontier_add(frontier *frntr, uint8_t depth, uint64_t hash,
                            char *url);
frontier_item *frontier_peek(frontier *frntr);
frontier_item *frontier_pop(frontier *frntr);

#endif
