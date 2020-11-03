#ifndef ASSIGNMENT3_LINKED_LIST_H
#define ASSIGNMENT3_LINKED_LIST_H

#include "error_handle.h"

struct list_node {
    void *data;
    struct list_node *next;
};

typedef struct list_node list_node;

list_node *CreateLinkedList();

void AddNode(list_node *head, void *data);

void *GetNext(list_node *node);

int Contains(list_node *head, void *data);

void DeleteNode(list_node *head, list_node *node);

void DeleteLinkedList(list_node *head);

int GetLength(list_node *head);

#endif
