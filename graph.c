#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graph.h"

vertex *createVertexNode();

vertex *CreateVertex(void *data) {
    vertex *newNode = createVertexNode();
    newNode->data = data;
    return newNode;
}

void AddEdge(vertex *from, vertex *to) {
    if (!Contains(from->edges, to)) {
        AddNode(from->edges, to);
    }
}

int HasEdge(vertex *from, vertex *to) {
    if (Contains(from->edges, to)) {
        return 1;
    } else {
        return 0;
    }
}

void *GetData(vertex *vertex) {
    if (vertex == NULL) {
        return NULL;
    }
    return vertex->data;
}

void DeleteVertex(vertex *vertex) {
    DeleteLinkedList(vertex->edges);
    free(vertex);
}

vertex *createVertexNode() {
    vertex *newNode = malloc(sizeof(vertex));
    ValidateMemoryAllocationError(newNode);
    newNode->data = NULL;
    newNode->edges = CreateLinkedList();
    return newNode;
}
