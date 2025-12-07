#include "charlist.h"


CharList* init_char_list() {
    CharList *list = malloc(sizeof(CharList));
    list->length = 0;
    list->head = NULL;
    list->tail = NULL;
    return list;
};

struct CharNode* new_char_node(char c) {
    struct CharNode* node = malloc(sizeof(struct CharNode));
    node->data = c;
    node->next = NULL;
    return node;   
}

void append_char(CharList *list, char c) {
    if (list->head == NULL) {
        list->head = new_char_node(c);
        list->tail = list->head;
        list->length++;
        return;
    }
    struct CharNode* node = new_char_node(c);
    list->tail->next = node;
    list->tail = list->tail->next;
    list->length++;
};

char* resolve(CharList *list) {
    if (list->length == 0) {
        // return '\0';
        return NULL;
    }
    char* res = malloc(sizeof(char)*list->length);
    struct CharNode* curr = list->head;
    struct CharNode* toFree;
    int i = 0;
    while(curr != NULL) {
        res[i++] = curr->data;
        toFree = curr;
        curr = curr->next;
        free(toFree);
    }
    // res[i] = '\0';
    list->length = 0;
    list->head = NULL; list->tail = NULL;
    return res;
};

void deallocate_char_list(CharList *list) {
    struct CharNode* curr = list->head;
    struct CharNode* toFree;
    while(curr != NULL) {
        toFree = curr;
        curr = curr->next;
        free(toFree);
    }
    free(list);
}