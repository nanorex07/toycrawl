#ifndef MEMTABLE_H
#define MEMTABLE_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct {
    uint64_t *table;
    uint32_t length;
    uint32_t size;
} memtable;

memtable *new_memtable(uint32_t size);
void memtable_add(memtable *table, uint64_t item);
bool memtable_contains(memtable *table, uint64_t item);
bool memtable_full(memtable *table);
void memtable_free(memtable *table);
void memtable_sort(memtable *table);

#endif

// ---------- implementation ----------

#ifndef MEMTABLE_IMPL

static int cmp_uint64_asc(const void *a, const void *b) {
    uint64_t x = *(const uint64_t *)a;
    uint64_t y = *(const uint64_t *)b;
    return (x > y) - (x < y);
}

memtable *new_memtable(uint32_t size) {
    memtable *mtable = (memtable *)malloc(sizeof(memtable));
    mtable->table = (uint64_t *)calloc(sizeof(uint64_t), size);
    mtable->length = 0;
    mtable->size = size;
    return mtable;
}

void memtable_free(memtable *table) {
    free(table->table);
    free(table);
}

bool memtable_full(memtable *table) { return table->length >= table->size; }

void memtable_add(memtable *table, uint64_t item) {
    if (memtable_full(table))
        return;
    table->table[table->length++] = item;
    memtable_sort(table);
}

bool memtable_contains(memtable *table, uint64_t item) {
    for (uint32_t i = 0; i < table->length; i++) {
        if (table->table[i] == item)
            return true;
    }
    return false;
}

void memtable_sort(memtable *table) {
    qsort(table->table, table->length, sizeof(table->table[0]), cmp_uint64_asc);
}

#endif
