#include <memtable.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

memtable *new_memtable(size_t size) {
    memtable *mtable = malloc(sizeof(memtable));
    mtable->table = calloc(sizeof(uint64_t), size);
    mtable->length = 0;
    mtable->size = size;
    return mtable;
}

void free_memtable(memtable *table) {
    free(table->table);
    free(table);
}

bool memtable_full(memtable *table) { return table->length >= table->size; }

void memtable_add(memtable *table, uint64_t item) {
    if (memtable_full(table))
        return;
    table->table[table->length++] = item;
}

bool memtable_contains(memtable *table, uint64_t item) {
    for (size_t i = 0; i < table->length; i++) {
        if (table->table[i] == item)
            return true;
    }
    return false;
}

static int cmp_uint64_asc(const void *a, const void *b) {
    uint64_t x = *(const uint64_t *)a;
    uint64_t y = *(const uint64_t *)b;
    return (x > y) - (x < y);
}

void memtable_sort(memtable *table) {
    qsort(table->table, table->length, sizeof(table->table[0]), cmp_uint64_asc);
}
