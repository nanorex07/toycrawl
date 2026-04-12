#ifndef LOOKUP_H
#define LOOKUP_H

#include "logger.h"
#include "memtable.h"
#include "utils.h"
#include "assert.h"
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <inttypes.h>

#define MANIFEST_FILENAME ".manifest"
#define SEGMENT_BYTES 25 // 1+4+4+8+8
#define SEGMENT_FILENAME_LEN 20
#define MEMTABLE_SIZE 3

typedef struct {
    uint8_t level;
    uint32_t id;
    uint32_t count;
    uint64_t min_hash;
    uint64_t max_hash;
} segment;

typedef struct {
    segment *items;
    uint32_t count;
    uint32_t capacity;
} manifest;

typedef struct {
    char *lookup_dir;
    manifest *manifest;
    uint32_t num_segments;
    memtable *memtable;
} lookup;

lookup *lookup_init(char *lookup_dir);
void lookup_free(lookup *lo);
bool lookup_insert(lookup *lo, uint64_t hash);
bool lookup_find(lookup *lo, uint64_t hash);

#endif

// ---------- implementation ----------

#ifndef LOOKUP_IMPL

lookup *lookup_init(char *lookup_dir) {
    lookup *lo = (lookup *)malloc(sizeof(lookup));
    lo->lookup_dir = (char *)malloc(strlen(lookup_dir) + 1);
    strcpy(lo->lookup_dir, lookup_dir);

    lo->manifest = (manifest *)malloc(sizeof(manifest));
    lo->manifest->count = 0;
    lo->manifest->capacity = 0;
    lo->manifest->items = NULL;

    lo->memtable = new_memtable(MEMTABLE_SIZE);

    // read segments from manifest
    int manifest_filename_len =
        strlen(lookup_dir) + strlen(MANIFEST_FILENAME) + 2;
    char manifest_path[manifest_filename_len];
    snprintf(manifest_path, manifest_filename_len, "%s/%s", lookup_dir,
             MANIFEST_FILENAME);
    log_error(manifest_path);
    FILE *manifest = fopen(manifest_path, "rb");
    if (manifest) {
        uint8_t segment_data[SEGMENT_BYTES] = {0};
        while (fread(segment_data, 1, SEGMENT_BYTES, manifest) ==
               SEGMENT_BYTES) {
            segment segment = {
                .level = (uint8_t)read_uintN_le(segment_data, 1),
                .id = (uint32_t)read_uintN_le(segment_data + 1, 4),
                .count = (uint32_t)read_uintN_le(segment_data + 5, 4),
                .min_hash = (uint64_t)read_uintN_le(segment_data + 9, 8),
                .max_hash = (uint64_t)read_uintN_le(segment_data + 17, 8)
            };
            DA_APPEND(lo->manifest, segment);
            log_info("segment: level=%u, id=%u, count=%u, min_hash=%llu, max_hash=%llu",
                     segment.level, segment.id, segment.count, segment.min_hash, segment.max_hash);
        }
    }
    fclose(manifest);

    return lo;
}

void lookup_free(lookup *lo) {
    free(lo->lookup_dir);
    free(lo->manifest->items);
    free(lo->manifest);
    memtable_free(lo->memtable);
    free(lo);
}

void _write_segment_to_manifest(lookup *lo) {
    int manifest_filename_len =
        strlen(lo->lookup_dir) + strlen(MANIFEST_FILENAME) + 7;

    char temp_manifest_path[manifest_filename_len];
    char manifest_path[manifest_filename_len];
    snprintf(manifest_path, manifest_filename_len, "%s/%s", lo->lookup_dir,
             MANIFEST_FILENAME);
    snprintf(temp_manifest_path, manifest_filename_len, "%s/%s.temp", lo->lookup_dir,
             MANIFEST_FILENAME);

    FILE *temp_manifest = fopen(temp_manifest_path, "wb");
    assert(temp_manifest!=NULL);

    for (uint32_t i = 0; i < lo->manifest->count; i++) {
        segment* curr = &lo->manifest->items[i];
        uint8_t segment_data[SEGMENT_BYTES] = {0};
        write_uintN_le(segment_data, curr->level, 1);
        write_uintN_le(segment_data+1, curr->id, 4);
        write_uintN_le(segment_data+5, curr->count, 4);
        write_uintN_le(segment_data+9, curr->min_hash, 8);
        write_uintN_le(segment_data+17, curr->max_hash, 8);
        fwrite(segment_data, 1, SEGMENT_BYTES, temp_manifest);
    }
    fflush(temp_manifest);
    fsync(fileno(temp_manifest));
    rename(temp_manifest_path, manifest_path);
    fclose(temp_manifest);
}

void _flush_segment(lookup *lo) {
    // flush a new segment to disk and upsert manifest
    // Create a new segment id by taking last segment in manifest and incrementing it
    uint32_t new_id = (lo->manifest->count == 0) ? 1 : lo->manifest->items[lo->manifest->count - 1].id + 1;
    segment seg = {
        .level = 0,
        .id = new_id,
        .count = lo->memtable->length,
        .min_hash = lo->memtable->table[0],
        .max_hash = lo->memtable->table[lo->memtable->length - 1],
    };

    char segment_path[strlen(lo->lookup_dir) + 1 + SEGMENT_FILENAME_LEN];
    sprintf(segment_path, "%s/seg_%" PRIu32 ".bin", lo->lookup_dir, new_id);

    FILE *segment_file = fopen(segment_path, "wb");
    assert(segment_file!=NULL);
    log_warning("Flushing segment: %s", segment_path);

    fwrite(lo->memtable->table, sizeof(uint64_t), lo->memtable->length, segment_file);
    fflush(segment_file);
    fsync(fileno(segment_file));
    fclose(segment_file);
    DA_APPEND(lo->manifest, seg);
}

void _reset_memtable(lookup *lo) {
    memtable_free(lo->memtable);
    lo->memtable = new_memtable(MEMTABLE_SIZE);
}

bool lookup_insert(lookup *lo, uint64_t hash) {
    log_info("Adding Hash: %" PRIu32 "\n", hash);
    if (memtable_full(lo->memtable)) {
        _flush_segment(lo);
        _write_segment_to_manifest(lo);
        _reset_memtable(lo);
    }
    memtable_add(lo->memtable, hash);
    return true;
}

bool lookup_find(lookup *lo, uint64_t hash) { return false; }

#endif
