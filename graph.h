/**
 * @author Harsh Rawat, harsh-rawat, hrawat2
 * @author Sidharth Gurbani, gurbani, gurbani
 */

#ifndef ASSIGNMENT3_GRAPH_H
#define ASSIGNMENT3_GRAPH_H

#include "linked_list.h"

struct vertex {
    //data stored in the vertex of graph
    void *data;
    //list of all the edges from this vertex. Each node of the list is a vertex.
    list_node *edges;
};

typedef struct vertex vertex;

vertex *CreateVertex(void *data);

void AddEdge(vertex *from, vertex *to);

void *GetData(vertex *vertex);

#endif
