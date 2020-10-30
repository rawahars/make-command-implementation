#include <stdio.h>
#include <stdlib.h>
#include "graph.h"

node* createNode();

node* CreateVertex(void* data){
    node* newNode = createNode();
    newNode->data = data;
    return newNode;
}

void AddEdge(node* from, node* to){
    if(!Contains(from->edges, to)){
        AddNode(from->edges, to);
    }
}

int HasEdge(node* from, node* to){
    if(Contains(from->edges, to)){
        return 1;
    } else {
        return 0;
    }
}

void* GetData(node* vertex){
    if(vertex == NULL){
        return NULL;
    }
    return vertex->data;
}

void DeleteVertex(node* vertex){
    DeleteLinkedList(vertex->edges);
    free(vertex);
}

node* createNode(){
    node* newNode = malloc(sizeof(node));
    newNode->data = NULL;
    newNode->edges = CreateLinkedList();
    return newNode;
}