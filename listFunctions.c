#include <stdio.h>
#include <stdlib.h>
#include "string.h"
#include "defs.h"

//Function to initiliaze ListType structs
void initList(ListType *llist) {
	llist->head = NULL;
	llist->tail = NULL;
	llist->size = 0;
}

//Function to add a newNode to the list newNode->data = s
void addStringToList(ListType *list, char* s) {
  NodeType *newNode;

  newNode = malloc(sizeof(NodeType));
  newNode->data = s;

  if (list->head == NULL) {
    list->head = newNode;
    list->tail = newNode;
    newNode->next = NULL;
    newNode->prev = NULL;
  } else {
    (list->tail)->next = newNode;
    newNode->prev = list->tail;
    list->tail = newNode;
    newNode->next = NULL;
  }

  list->size += 1;
}

//Function to print the data of a list
void printList(ListType *list) {
  NodeType *currNode = list->head;
	while (currNode != NULL) {
		printf("%s\n", currNode->data);
    currNode = currNode->next;
	}
}

//Function to free dynamically allocated data for a list
void freeList(ListType *list) {
	if (list->size > 0) {
	  NodeType *currNode = list->tail;
		while (currNode->prev != NULL) {
	    if (currNode->next != NULL) {
	      free(currNode->next);
	    }
	    free(currNode->data);
	    currNode = currNode->prev;
		}
	  if(list->head != NULL) {
	    free(list->head);
	  }
	}
  free(list);

}
