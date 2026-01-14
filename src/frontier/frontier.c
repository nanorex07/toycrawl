#include "frontier.h"

frontier *initialize_frontier(char *frontier_folder) {
    frontier *frntr = malloc(sizeof(frontier));

    int filename_len = MAX_FILE_NAME_LENGTH + strlen(frontier_folder) + 1;
    char frontier_log_filename[filename_len],
        frontier_offset_filename[filename_len];

    snprintf(frontier_log_filename, filename_len, "%s/%s", frontier_folder,
             FRNTR_LOG_FILENAME);
    snprintf(frontier_offset_filename, filename_len, "%s/%s", frontier_folder,
             FRNTR_OFFSET_FILENAME);

    frntr->frontier_log = open_file_with_error(frontier_log_filename, "a+");
    frntr->frontier_offset = open_create_file(frontier_offset_filename, "r+b");

    // read frontier offset from file
    uint8_t bytes[4];
    if (fread(bytes, 1, 4, frntr->frontier_offset) != 4) {
        frntr->read_head = 0;
    } else {
        frntr->read_head = read_uintN_le(bytes, 4);
    }
    log_warning("Read HEAD: %lld", frntr->read_head);
    return frntr;
}

void free_frontier(frontier *frntr) { free(frntr); }

void free_frontier_item(frontier_item *item) {
    if (!item) {
        log_warning("free_frontier_item | Nothing to free");
        return;
    }
    free(item->url);
    free(item);
}

void log_frontier_item(frontier_item *item) {
    log_info("Item{Dph: %d, N: %d, Hash: %lld, Url: %s}", item->depth,
             item->url_len, item->url_hash, item->url);
}

frontier_item *frontier_add(frontier *frntr, uint8_t depth, uint64_t hash,
                            char *url) {
    frontier_item *item = malloc(sizeof(frontier_item));
    if (item == NULL)
        return NULL;

    item->depth = depth;
    item->url = url;
    item->url_hash = hash;
    item->url_len = strlen(url);

    uint8_t header[FRNTR_HEADER_LEN];
    write_uintN_le(header, depth, FRNTR_DEPTH_BYTES);
    write_uintN_le(header + FRNTR_DEPTH_BYTES, item->url_len,
                   FRNTR_URLLEN_BYTES);
    write_uintN_le(header + FRNTR_DEPTH_BYTES + FRNTR_URLLEN_BYTES,
                   item->url_hash, FRNTR_URLHASH_BYTES);

    fwrite(header, 1, FRNTR_HEADER_LEN, frntr->frontier_log);
    fwrite((uint8_t *)url, 1, item->url_len, frntr->frontier_log);

    return item;
}

frontier_item *_frontier_read_one(frontier *frntr) {
    fseek(frntr->frontier_log, frntr->read_head, SEEK_SET);
    frontier_item *item = malloc(sizeof(frontier_item));
    if (item == NULL)
        return NULL;

    uint8_t header[FRNTR_HEADER_LEN];

    size_t read = fread(header, 1, FRNTR_HEADER_LEN, frntr->frontier_log);
    if (read != FRNTR_HEADER_LEN)
        return NULL;

    item->depth = read_uintN_le(header, FRNTR_DEPTH_BYTES);
    item->url_len =
        read_uintN_le(header + FRNTR_DEPTH_BYTES, FRNTR_URLLEN_BYTES);
    item->url_hash = read_uintN_le(
        header + FRNTR_DEPTH_BYTES + FRNTR_URLLEN_BYTES, FRNTR_URLHASH_BYTES);

    item->url = malloc(item->url_len + 1);
    fread(item->url, 1, item->url_len, frntr->frontier_log);
    item->url[item->url_len] = '\0';

    return item;
}

frontier_item *_copy_frontier_item(frontier_item *item) {
    frontier_item *copy = malloc(sizeof(frontier_item));
    copy->depth = item->depth;
    copy->url_len = item->url_len;
    copy->url_hash = item->url_hash;
    copy->url = malloc(sizeof(char) * item->url_len);
    strncpy(copy->url, item->url, item->url_len);
    return copy;
}

frontier_item *frontier_peek(frontier *frntr) {
    if (frntr->peek_cache == NULL) {
        frontier_item *item = _frontier_read_one(frntr);
        if (item == NULL)
            return NULL;
        frntr->peek_cache = item;
    }
    return _copy_frontier_item(frntr->peek_cache);
}

frontier_item *frontier_pop(frontier *frntr) {

    frontier_item *item;
    if (frntr->peek_cache != NULL) {
        log_info("Using Cache!");
        item = frntr->peek_cache;
        frntr->peek_cache = NULL;
    } else {
        item = _frontier_read_one(frntr);
        if (item == NULL)
            return NULL;
    }

    frntr->read_head += FRNTR_HEADER_LEN + item->url_len;

    rewind(frntr->frontier_offset);
    size_t bytes_written =
        fwrite(&frntr->read_head, 4, 1, frntr->frontier_offset);

    frontier_item *copy = _copy_frontier_item(item);
    free_frontier_item(item);
    return copy;
}

bool frontier_empty(frontier *frntr) {
    frontier_item *item = frontier_peek(frntr);
    if (item == NULL) {
        return true;
    }
    free_frontier_item(item);
    return false;
}
