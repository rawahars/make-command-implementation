/**
 * @author Harsh Rawat, harsh-rawat, hrawat2
 * @author Sidharth Gurbani, gurbani, gurbani
 */

#include <stdlib.h>
#include <string.h>
#include "graph.h"

vertex *createVertexNode();

/**
 * This method is used to create a new vertex with the given data
 * */
vertex *CreateVertex(void *data) {
    vertex *newNode = createVertexNode();
    newNode->data = data;
    return newNode;
}

/**
 * This method is used to add a edge from a given vertex to a target vertex
 * */
void AddEdge(vertex *from, vertex *to) {
    if (!Contains(from->edges, to)) {
        AddNode(from->edges, to);
    }
}

/**
 * This method is used to get the data from the given vertex
 * */
void *GetData(vertex *vertex) {
    if (vertex == NULL) {
        return NULL;
    }
    return vertex->data;
}

/**
 * This is an internal method which is used to create a vertex node by allocating the required memory
 * */
vertex *createVertexNode() {
    vertex *newNode = malloc(sizeof(vertex));
    ValidateMemoryAllocationError(newNode);
    newNode->data = NULL;
    newNode->edges = CreateLinkedList();
    return newNode;
}
