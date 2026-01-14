#ifndef MEMTABLE_H
#define MEMTABLE_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct {
    uint64_t *table;
    size_t length;
    size_t size;
} memtable;

memtable *new_memtable(size_t size);
void memtable_add(memtable *table, uint64_t item);
bool memtable_contains(memtable *table, uint64_t item);
bool memtable_full(memtable *table);
void free_memtable(memtable *table);
void memtable_sort(memtable *table);

#endif
