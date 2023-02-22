/*
 ============================================================================
 Name        : a3.c
 Author      : Omar Khan
 Version     : 1.0
 Copyright   : Copyright: Omar Khan, 2021
 Description : A program that allows the user to search .csv data files for pokemon
               and save the results in to another file
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pthread.h"
#include "defs.h"
#include <pthread.h>
#include <semaphore.h>

//numQueries is a global variable for the purpose of using in multiple threads
volatile int numQueries = 0;
sem_t mutex;

int main(int argc, char const *argv[]) {
  //declaring/initializing ListType structs, threads, semaphore, and helper variables
  int exitStatus = 0;
  char fileName[MAX_STR];
  pthread_t t1, t2;
  ListType *pokemonInMem;
  ListType *newFiles;
  ParameterType *para;
  pokemonInMem = malloc(sizeof(ListType));
  newFiles = malloc(sizeof(ListType));
  initList(pokemonInMem);
  initList(newFiles);

  if (sem_init(&mutex, 0, 1) < 0) {
    printf("Error: on semaphore init.\n");
    exit(1);
  }

  //Getting user input for fileName and making sure there are no errors opening the file
  FILE *in;
  printf("Please enter the name of the file containing the pokemon descriptions:\n>> ");
  scanf("%s", fileName);
  in = fopen(fileName, "r");
  while (!in) {
    printf("Error: could not open file \"%s\".\n", fileName);
    printf("Please enter the name of the file containing the pokemon descriptions\n>> ");
    scanf("%s", fileName);
    in = fopen(fileName, "r");
  }
  printf("Searching file: %s\n", fileName);
  fclose(in);

  //Starting the query program
  printf("\nWelcome to the Pokemon query program!\n");
  printf("=====================================\n");
  while (exitStatus != 3) {
    //Looping until the user enters 3 (quit) in the query() function
    exitStatus = query();
    if (exitStatus == 1) {
      //User has selected option 1 (type search) so ask the user for search query
      printf("Enter a pokemon type to search:\n>> ");
      char search[15];
      scanf("%s", search);
      //Creating and initializing ParameterType to pass into pthread_create
      para = malloc(sizeof(ParameterType));
      para->pokemon = pokemonInMem;
      para->fileName = fileName;
      para->s = search;
      //Creating thread for typeSearch function
      pthread_create(&t1, NULL, typeSearch, para);
    } else if (exitStatus == 2) {
      //User has selected option 2 (save results) so ask the user for new file name
      char *newFile = malloc(15 * sizeof(char));
      FILE *out = NULL;
      while (!out) {
        printf("Enter a file name to contain the accumulated query results:\n>> ");
        scanf("%s", newFile);
        out = fopen(newFile, "w");
        if (!out) {
          printf("Unable to create the new file. Please enter the name of the file again.\n");
        }
      }
      fclose(out);
      //Creating and initializing ParameterType to pass into pthread_create
      para = malloc(sizeof(ParameterType));
      para->pokemon = pokemonInMem;
      para->fileName = newFile;
      //Creating thread for saveResult function
      pthread_create(&t2, NULL, saveResult, para);
      //Adding new file name to newFiles list
      addStringToList(newFiles, newFile);
    }
  }

  //printing numQueries and newly created files
  printf("Number of successful queries: %d\n", numQueries);
  printf("New files created:\n");
  if (newFiles->size > 0) {
    printList(newFiles);
  } else {
    printf("None\n");
  }
  //freeing memory
  freeList(newFiles);
  freeList(pokemonInMem);
  if (para != NULL) {
    free(para);
  }

  printf("================> Goodbye!\n");
  return 0;
}

int query() {
  int choice;
  printf("Select an option from the menu:\n");
  printf("1) Type Search\n");
  printf("2) Save results\n");
  printf("3) Quit Program\n>> ");
  scanf("%d", &choice);
  return choice;
}

void *typeSearch(void* p) {
  //Locking the semaphore mutex
  sem_wait(&mutex);
  //Casting the parameter type and collecting the data from it
  ParameterType *parameters = ((ParameterType*)p);
  ListType* pokemon = parameters->pokemon;
  char* fileName = parameters->fileName;
  char* search = parameters->s;

  //Opening the file for reading
  FILE *in;
  char currLine[100];
  int count = 0;
  in = fopen(fileName, "r");
  //Loop reading the file line by line
  while(fgets(currLine, sizeof(currLine), in)) {
    char type1[15];
    char token[100];
    //Taking care not to botch currLine
    strcpy(token, currLine);

    //Reading until the first comma
    strtok(token, ",");
    //Reading from first to the second comma
    strtok(NULL, ",");
    //Reading from second to the third comma
    strcpy(type1, strtok(NULL, ","));
    //Comparing user search with type 1 of each pokemon
    if (strcmp(search,type1) == 0) {
      //User search matched this line so add line to pokemon list and increment count
      char *match = malloc(sizeof(char) * strlen(currLine));
      strcpy(match, currLine);
      addStringToList(pokemon, match);
      count++;
    }
  }
  //If count is greater than 0 means there was a successful query so increment numQueries
  if (count > 0) {
    numQueries += 1;
  }
  fclose(in);
  //Unlock the semaphore mutex
  sem_post(&mutex);
}

void *saveResult(void *p) {
  //Lock the semaphore mutex
  sem_wait(&mutex);
  //Casting the parameter type and collecting the data from it
  ParameterType *parameters = ((ParameterType*)p);
  ListType* pokemon = parameters->pokemon;
  char* fileName = parameters->fileName;

  //Open file for writing
  FILE *out = NULL;
  out = fopen(fileName, "w");

  //Loop through pokemon list
  NodeType *currNode = pokemon->head;
  while (currNode != NULL) {
    //Write to out file each nodes data
    fwrite(currNode->data, sizeof(char), strlen(currNode->data), out);
    currNode = currNode->next;
  }
  //Close file and unlock semaphore mutex
  fclose(out);
  sem_post(&mutex);
}
