#ifndef CHARLIST_H
#define CHARLIST_H
#include <stddef.h>
#include <stdlib.h>


struct CharNode {
    char data;
    struct CharNode *next;
};

typedef struct {
    struct CharNode *head;
    struct CharNode *tail;
    size_t length;
} CharList;

CharList* init_char_list();
void append_char(CharList *list, char c);
char* resolve(CharList *list);
void deallocate_char_list(CharList* list);

#endif