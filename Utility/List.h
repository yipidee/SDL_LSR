#ifndef _LIST_H
#define _LIST_H

#include <stdbool.h>

// a common function used to free malloc'd objects
typedef void (*FreeFunction)(void *);

typedef bool (*ListIterator)(void *);

typedef struct _listNode {
    void* data;
    struct _listNode* next;
} ListNode;

typedef struct {
    int logicalLength;
    int elementSize;
    ListNode* head;
    ListNode* tail;
    FreeFunction freeFn;
} List;

void List_new(List* list, int elementSize, FreeFunction freeFn);
void List_destroy(List* list);

void List_prepend(List* list, void* element);
void List_append(List* list, void* element);
int List_size(List* list);

void List_forEach(List* list, ListIterator iterator);
void List_head(List* list, void* element, bool removeFromList);
void List_tail(List* list, void* element);

#endif _LIST_H
