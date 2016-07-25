#include <stdlib.h>
#include <string.h>
#include "List.h"

//create new list
void List_new(List* list, int elementSize, FreeFunction freeFn)
{
    list->logicalLength = 0;
    list->elementSize = elementSize;
    list->head = NULL;
    list->tail = NULL;
    list->freeFn = freeFn;
}

//deallocate list memory
void List_destroy(List* list)
{
    ListNode* current;
    while(list->head != NULL)
    {
        current = list->head;
        list->head=current->next;

        if(list->freeFn) list->freeFn(current->data);

        free(current->data);
        free(current);
    }
}

//add element to start of list
void List_prepend(List* list, void* element)
{
    ListNode* newFront = malloc(sizeof(ListNode));
    newFront->data = malloc(list->elementSize);
    memccpy(newFront->data, element, list->elementSize);

    newFront->next = list->head;
    list->head = newFront;

    if(!list->tail) list->tail=list->head;

    list->logicalLength++;
}

//add element to start of list
void List_append(List* list, void* element)
{
    ListNode* newEnd = malloc(sizeof(ListNode));
    newEnd->data = malloc(list->elementSize);
    newEnd->next = NULL;

    memccpy(newEnd->data, element, list->elementSize);

    if(list->logicalLength == 0)
    {
        list->head=list->tail=newEnd;
    }else
    {
        list->tail->next = newEnd;
        list->tail = newEnd;
    }

    list->logicalLength++;
}

int List_size(List* list)
{
    return list->logicalLength;
}

void List_forEach(List* list, ListIterator iterator)
{
    ListNode* node = list->head;
    bool res = true;

    while(node!=NULL)
    {
        result = iterator(node->data);
        node = node->next;
    }
}

//write the data from list head into element, deleting head if specified
void List_head(List* list, void* element, bool removeFromList)
{
    ListNode* node = list->head;
    memccpy(element, node, list->elementSize);

    if(removeFromList)
    {
        list->head = node->next;
        list->logicalLength--;

        free(node->data);
        free(node);
    }
}

void List_tail(List* list, void* element)
{
    ListNode* node = list->head;
    memccpy(element, node, list->elementSize);
}
