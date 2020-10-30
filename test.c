#include <stdio.h>
#include "graph.h"
#include "linked_list.h"

typedef struct{
    char data;
} testData;

void printList(list_node* head);

int main(void) {

    testData a;
    testData b;
    testData c;
    testData d;
    testData e;
    a.data = 'a';
    b.data = 'b';
    c.data = 'c';
    d.data = 'd';
    e.data = 'e';

    vertex* A = CreateVertex(&a);
    vertex* B = CreateVertex(&b);
    AddEdge(A,B);
    vertex* C = CreateVertex(&c);
    AddEdge(B,C);
    vertex* D = CreateVertex(&d);
    AddEdge(C,A);
    AddEdge(A,D);

    printf("Hello World\n");

    return 1;
}
