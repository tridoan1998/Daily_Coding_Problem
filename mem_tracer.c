#include<stdio.h>
#include<stdlib.h>
//#include<sys/wait.h>
#include<string.h>
#include<unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>


/**
@Author: Tri Doan, Tam Huynh
@Couse: CS 149 SJSU Spring 2020
@Description: Implement a program mem_tracer.c that reads all lines from an input file, in a similar as way you did for assignment 3.
 However, read all lines into an array of type char ** . For example, for the input file provided as an instance for assignment 3 your 
 char **array will store:
*/

//create linked list, global varible
struct Node 
{ 
    int index;
    char array[100];

    struct Node *next; 
}; 

//still don't know how the index of the array ++ or what **********
//function to create the linekd list and add data into it 
void add_node_linkedlist(int index_to_add, char **array)
{
  // Allocate memory for node 
  struct Node* new_node = (struct Node*)malloc(sizeof(struct Node));
  new_node->index = index_to_add;                      //match with line 18

   strcpy(new_node->array, array[index_to_add]);        //match with line 19

  //(*node)->next= new_node;
  new_node->next = NULL;
}

//print all datas in the linked list
void PrintNodes(struct Node *head)
{
  //base case for recursive print
  if(head == NULL)
  {
      return;
  }

  PrintNodes(head->next);
  
  printf("Array index: %d   ", head->index);  
  printf("Value in the array: %d   ", head->array);  

}

void* REALLOC(size_t size, char **char)
{
  void *newptr;
    int msize;
    msize = getsize(char);
    printf("msize=%d\n", msize);
    if (size <= msize)
        {
          return char;
        }
    newptr = malloc(size);
    memcpy(newptr, char, msize);
    free(char);
    return newptr;
}


void* MALLOC(size_t sz, char **array)
{
  static size_t next_index = 0;
  static pthread_mutex_t lock;

    void *mem;
    pthread_mutex_lock(&lock);
    if(sizeof(array) - next_index < sz)
    {
        pthread_mutex_unlock(&lock);
        return NULL;
    }

    mem = &array[next_index];
    next_index += sz;
    pthread_mutex_unlock(&lock);
    return mem;
}

void FREE(void* p)
{
    size_t * in = p;
    if (in) 
    {
        --in;
        free(in);
    }
}

/*
void Dup2(int index_to_add, char **array)
{
  size_t n = sizeof(array);
  int counter = 0;
  FILE *fp;
  fp = fopen("memtrace.out", "w+");
  while(counter < n)
  {
    fwrite(index_to_add, sizeof(int), 1, fp);
    fwrite(array[counter], sizeof(array), 1, fp);
  }
  fclose(fp);
}
*/

//main method to evolt the command, try to read in the file if successful
int main(int argc, char **argv)
{
  FILE *file;

  file = fopen(argv[1], "r");

  if(file == NULL)
  {
    printf("File can't be open \n");
    exit(1);
  }

  //pointer of pointer
  char **character =NULL; 
  // Dynamically allocate memory using malloc()  
  //character = (char**)malloc(100 * sizeof(char*));
  MALLOC(100 * sizeof(char*), character); 

  int index =0;

  char temp[100];
  //input all file data into the character pointer 
  while (!feof(file))
  {
    if(fgets(temp, 20, file) != NULL)
      {
          //need to create a malloc for character[index] *******
         // character[index] = (char*)malloc(100 * sizeof(char)); 
            character[index] =  MALLOC(100 * sizeof(char*), character); 

          strcpy(character[index], temp);
      
      }
    index++;

    //need to realloc() if the file is more than 100 ********
    if(index >= (100 * sizeof(char)))
      {
        character = realloc(character, 100 * sizeof(char));
      }

  }

    //create an object linked list
    //struct Node *node = (struct Node*)malloc(sizeof(struct Node));
    struct Node *node =(struct Node*)MALLOC(100 * sizeof(struct Node), character);
    node = NULL;

    unsigned char_size = sizeof(char);

    for (int i=0; i <= 100; i++)
    {
       add_node_linkedlist(index, character); 
    }
    
    PrintNodes(node);
    
    for (int i=0; i <= 100; i++)
    {
        printf("Array[%d] = %s", index , character);  
    }

    //use dup2 here
    int input_fds = open("memtrace.out", O_RDONLY);
   
    dup2(input_fds, STDIN_FILENO);
    if(dup2(input_fds, STDIN_FILENO) < 0) 
    {
      printf("Unable to duplicate file descriptor.");
      exit(EXIT_FAILURE);
    }
    return 0;
}


