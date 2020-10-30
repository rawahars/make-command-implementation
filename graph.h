#ifndef ASSIGNMENT3_GRAPH_H
#define ASSIGNMENT3_GRAPH_H

#include "linked_list.h"

struct node{
    void* data;
    list_node* edges;
};

typedef struct node node;

node* CreateVertex();
void AddEdge(node* from, node* to);
int HasEdge(node* from, node* to);
void* GetData(node* vertex);
void DeleteVertex(node* vertex)

#endif
