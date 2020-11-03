#include <stdlib.h>
#include "linked_list.h"

list_node *createNode();

list_node *CreateLinkedList() {
    return createNode();
}

void AddNode(list_node *head, void *data) {
    list_node *temp = head->next;
    while (temp != NULL) {
        head = temp;
        temp = head->next;
    }
    head->next = createNode();
    head = head->next;
    head->data = data;
}

void *GetNext(list_node *node) {
    if (node->next == NULL) {
        return NULL;
    } else {
        return node->next->data;
    }
}

int Contains(list_node *head, void *data) {
    while (head != NULL) {
        if (head->data == data) {
            return 1;
        }
        head = head->next;
    }
    return 0;
}

void DeleteNode(list_node *head, list_node *node) {
    list_node *temp = head->next;
    while (temp != NULL && temp != node) {
        head = temp;
        temp = head->next;
    }
    if (temp != NULL) {
        head->next = temp->next;
        free(temp);
    }
}

list_node *createNode() {
    list_node *head = malloc(sizeof(struct list_node));
    ValidateMemoryAllocationError(head);
    head->next = NULL;
    head->data = NULL;
    return head;
}

void DeleteLinkedList(list_node *head) {
    list_node *fast = head;
    while (fast != NULL) {
        fast = head->next;
        free(head);
        head = fast;
    }
}

int GetLength(list_node *head) {
    int len = -1;// Since we are starting with head which is a dummy node
    while (head != NULL) {
        head = head->next;
        len++;
    }
    return len;
}
