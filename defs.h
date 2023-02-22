#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>

#define MAX_STR 20

typedef struct Node {
	char *data;
	struct Node *prev;
	struct Node *next;
} NodeType;

typedef struct {
	NodeType *head;
	NodeType *tail;
	int size;
} ListType;

typedef struct {
  ListType *pokemon;
  char *fileName;
	char *s;
} ParameterType;

int main(int argc, char const *argv[]);
int query();
void* typeSearch(void*);
void* saveResult(void*);
void initList(ListType*);
void addStringToList(ListType* l, char* s);
void printList(ListType* l);
void freeList(ListType* l);
