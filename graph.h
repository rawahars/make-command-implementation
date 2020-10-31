#ifndef ASSIGNMENT3_GRAPH_H
#define ASSIGNMENT3_GRAPH_H

#include "linked_list.h"

struct vertex{
    void* data;
    list_node* edges;
};

typedef struct vertex vertex;

vertex* CreateVertex(void* data);
void AddEdge(vertex* from, vertex* to);
int HasEdge(vertex* from, vertex* to);
void* GetData(vertex* vertex);
void DeleteVertex(vertex* vertex);
int DetectCycleInGraph(vertex* start);

#endif
