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
    newNode->data = NULL;
    newNode->edges = CreateLinkedList();
    return newNode;
}

int isVertexInList(vertex* curr_vertex, list_node* list) {
    if (list->next == NULL) return 0;
    list_node* temp = list->next;
    rule* current_vertex_rule = (rule*)curr_vertex->data;
    while (temp != NULL/* && temp->data != NULL*/) {
        vertex* list_vertex = (vertex*)temp->data;
        rule* list_vertex_rule = (rule*)list_vertex->data;
        if (strcmp(current_vertex_rule->target_name, list_vertex_rule->target_name)==0) {
            return 1;
        }
        temp = temp->next;
    }
    return 0;
}

void addNodesToGlobal(list_node* global_list, list_node* curr_list) {
    list_node* temp = curr_list->next;
    while (temp != NULL/* && temp->data != NULL*/) {
        vertex* v = (vertex*)temp->data;
        if (isVertexInList(v, global_list) == 0) {
            AddNode(global_list, v);
        }
        temp = temp->next;
    }
}

void removeVertexFromList(list_node* list, vertex* curr_vertex) {
    list_node* temp = list->next;
    rule* current_vertex_rule = (rule*)curr_vertex->data;
    while (temp != NULL) {
        vertex* list_vertex = (vertex*) temp->data;
        rule* list_vertex_rule = (rule*)list_vertex->data;
        if (strcmp(current_vertex_rule->target_name, list_vertex_rule->target_name)==0) {
            list->next = temp->next;
            free(temp);
            break;
        }
        list = temp;
        temp = list->next;
    }
}

int DFS(vertex* curr_vertex, list_node* global_list, list_node* curr_list) {
    if (curr_vertex == NULL) return 0;
    if (isVertexInList(curr_vertex, global_list)) {
        return 0;
    }
    else if (isVertexInList(curr_vertex, curr_list)) {
        return 1;
    }
    list_node* edges = curr_vertex->edges;
    AddNode(curr_list, curr_vertex);
    vertex* next_vertex;
    while (edges != NULL) {
        next_vertex = (vertex*)GetNext(edges);
        if (DFS(next_vertex, global_list, curr_list) == 1) {
            return 1;
        }
        edges = edges->next;
    }
    if (isVertexInList(curr_vertex, global_list) == 0) {
        AddNode(global_list, curr_vertex);
    }
    removeVertexFromList(curr_list, curr_vertex);
    return 0;
}

int DetectCycleInGraph(list_node *start) {
    //To-do: Detect cycle and exit with error if one is found. You can use CycleInGraphError in error_handle module.
    list_node* temp = start->next;
    list_node* global_list = CreateLinkedList();
    while (temp != NULL) {
        list_node* curr_list = CreateLinkedList();
        vertex* v = (vertex*) temp->data;
        if (DFS(v, global_list, curr_list) == 1) {
            return 1;
        }
        temp = temp->next;
    }

    return 0;
}