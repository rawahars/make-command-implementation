/**
 * @author Harsh Rawat, harsh-rawat, hrawat2
 * @author Sidharth Gurbani, gurbani, gurbani
 */

#include <stdlib.h>
#include "linked_list.h"

list_node *createNode();

/**
 * This method is used to create and return a new linked list
 * */
list_node *CreateLinkedList() {
    return createNode();
}

/**
 * This method is used to add a new node with given data to the end of linked list
 * */
void AddNode(list_node *head, void *data) {
    list_node *temp = head->next;
    //find the end of list
    while (temp != NULL) {
        head = temp;
        temp = head->next;
    }
    //add new node at the end
    head->next = createNode();
    head = head->next;
    head->data = data;
}

/**
 * This method is used to get the data of next node in the linked list.
 * Next node is used since our first node is a dummy node
 * */
void *GetNext(list_node *node) {
    if (node->next == NULL) {
        return NULL;
    } else {
        return node->next->data;
    }
}

/**
 * This method is used to check if the given data exists inside the linked list referenced by head
 * */
int Contains(list_node *head, void *data) {
    while (head != NULL) {
        if (head->data == data) {
            return 1;
        }
        head = head->next;
    }
    return 0;
}

/**
 * This method is used to delete the given node from the linked list referenced by head
 * */
void DeleteNode(list_node *head, list_node *node) {
    list_node *temp = head->next;
    //find the node in list which is same as node. head is slow pointer and temp is fast pointer.
    while (temp != NULL && temp != node) {
        head = temp;
        temp = head->next;
    }
    //Change the pointers to delete the node
    if (temp != NULL) {
        head->next = temp->next;
        free(temp);
    }
}

/**
 * This method is used to create a new node by allocating memory
 * */
list_node *createNode() {
    list_node *head = malloc(sizeof(struct list_node));
    ValidateMemoryAllocationError(head);
    head->next = NULL;
    head->data = NULL;
    return head;
}

/**
 * This method is used to delete the entire linked list
 * */
void DeleteLinkedList(list_node *head) {
    list_node *fast = head;
    while (fast != NULL) {
        fast = head->next;
        free(head);
        head = fast;
    }
}

/**
 * This method is used to find the length of the linked list
 * */
int GetLength(list_node *head) {
    int len = -1;// Since we are starting with head which is a dummy node
    while (head != NULL) {
        head = head->next;
        len++;
    }
    return len;
}
