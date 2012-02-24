/*adlist.h - A generic doubly linked list implementation
 *
 */

#ifndef __ADLIST_H
#define __ADLIST_H

/* Node, List, and Iterator */

typedef struct listNode {
    struct listNode *prev;
    struct listNode *next;
    void *value;
}listNode;

typedef struct listIter {
    listNode *next;
    int direction;
}listIter;

typedef struct list {
    listNode *head;
    listNode *tail;
    void *(*dup)(void *ptr);
    void (*free)(void *ptr);
    int (*match)(void *ptr,void *key);
    unsigned long len;
}list;

/* Functions implemented as macros */
//to do
//
//
//

/* Prototypes */
list *listCreate(void);
void listRelease(list *);

/* Directions for iterators */
#define AL_START_HEAD 0
#define AL_START_TAIL 1

#endif /* __ADLIST_H__ */
