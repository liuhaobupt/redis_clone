/*adlist.c - A generic doubly linked list implementation
 *
 */

#include <stdlib.h>
#include "adlist.h"
#include "zmalloc.h"

list *listCreate(void)
{
    struct list *list;

    if((list = malloc(sizeof(*list))) == NULL)
        return NULL;
    list->head = list->tail = NULL;
    list->len = 0;
    list->dup = NULL;
    list->free = NULL;
    list->match = NULL;
    return list;
}

void listRelease(list *list)
{
    unsigned long len;
    listNode *current, *next;

    current = list->head;
    len = list->len;
    while(len--)
    {
        next = current->next;
        if(list->free) list->free(current->value);
        free(current);
        current = next;
    }
    free(list);
}
