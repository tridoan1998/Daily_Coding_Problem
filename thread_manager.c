#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdbool.h>
#include <time.h>
/**
@Author: Tri Doan, Tam Huynh
@Couse: CS 149 SJSU Spring 2020 Assignment 5
@Description: Write a simple multithreaded C program named thread_manager.c . Your program has two threads in which one thread creates a 
dynamic object of type THREADDATA (using malloc) and assigns it to pointer  p. Then the second thread deallocates it. This THREADDATA
 object stores the thread who was the creator of the object.
*/

//create linked list, global varible
struct LinkedList 
{ 
    char *value;
    struct LinkedList *next; 
}; 

typedef struct LinkedList *node; //Define node as pointer of data type struct LinkedList

struct LinkedList *head = NULL;
struct LinkedList *current = NULL;

//insert link at the first location
void insertFirst(char *buffer) 
{
   //create a link
   struct LinkedList *link = (struct LinkedList*) malloc(sizeof(struct LinkedList));
	
   link->value = buffer;

   //point it to old first node
   link->next = head;
   //point first to new first node
   head = link;
}

//display the list
void printList() 
{
   struct LinkedList *ptr = head;
   //start from the beginning
   printf("Head of linked list contains line: %s\n" , ptr->value);
}

//create the struct holding the thread to pass into the thread method
struct THREADDATA_STRUCT
{  
	pthread_t creator;
};
typedef struct THREADDATA_STRUCT THREADDATA;

THREADDATA* p=NULL;

//thread mutex lock for access to the log index
pthread_mutex_t tlock1 = PTHREAD_MUTEX_INITIALIZER;
//thread mutex lock for critical sections of allocating THREADDATA
pthread_mutex_t tlock2 = PTHREAD_MUTEX_INITIALIZER; 
//allows threads to suspend execution and relinquish the processors until some predicate on shared data is satisfied
pthread_cond_t cond1 = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond2 = PTHREAD_COND_INITIALIZER;
//A flag to indicate if the reading of input is complete, //so the other thread knows when to stop
bool is_reading_complete = false;

// variables to store date and time components
int hours, minutes, seconds, day, month, year;
// time_t is arithmetic time
long Logindex = 1;
time_t now;

/**
@Description: this is the thread runner, when it called, it will perform all the needed step for the requirement,
mallo the struct, assign it to pointer p, and then read user inputs to the linked list using the do while loop, and the lastly
is to deallocate pointer p. 
We will also print out the head of the linked list as every step take place along with the log index, PID, thread, date, and time. 
*/
void* thread_runner(void* thread)
{


	// Obtain current time
	// time() returns the current time of the system as a time_t value
	time(&now);
	// localtime converts a time_t value to calendar time and 
	// returns a pointer to a tm structure with its members
	// filled with the corresponding values
	struct tm *local = localtime(&now);

	hours = local->tm_hour;        // get hours since midnight (0-23)    
	minutes = local->tm_min;       // get minutes passed after the hour (0-59)    
	seconds = local->tm_sec;       // get seconds passed after minute (0-59)    
	day = local->tm_mday;          // get day of month (1 to 31)    
	month = local->tm_mon + 1;     // get month of year (0 to 11)    
	year = local->tm_year + 1900;  // get year since 1900

	pid_t PID = getpid();

 	pthread_t me;  
 	me = pthread_self();

 	pthread_mutex_lock(&tlock1);		// critical section starts  
    if (hours < 12)   		// before midday
	{
		printf("Logindex %ld, thread %ld, PID %d, %02d/%02d/%d, %02d:%02d:%02d am.\n", Logindex, me, PID, day, month, year , hours, minutes, seconds);
		printList();
		Logindex++;
	}
	else					// after midday
	{
		printf("Logindex %ld, thread %ld, PID %d, %02d/%02d/%d, %02d:%02d:%02d pm.\n", Logindex, me, PID, day, month, year , hours - 12, minutes, seconds);
		printList();
		Logindex++;
	} 	//one thread creates a dynamic object of type THREADDATA (using malloc) and assigns it to pointer  p
 	
 	

 	if (p==NULL) 
 	{    
 	p = malloc(sizeof(struct THREADDATA_STRUCT));

 		p->creator=me;  
 		printf("This is thread %ld with pointer p is not NULL (p=%p)\n",me,p);
 		    if (hours < 12)   		// before midday
			{
				printf("Logindex %ld, thread %ld, PID %d, %02d/%02d/%d, %02d:%02d:%02d am.\n", Logindex, me, PID, day, month, year , hours, minutes, seconds);
				printList();
				Logindex++;
			}
			else					// after midday
			{
				printf("Logindex %ld, thread %ld, PID %d, %02d/%02d/%d, %02d:%02d:%02d pm.\n", Logindex, me, PID, day, month, year , hours - 12, minutes, seconds);
				printList();
				Logindex++;
			}
 	}  

	//Thread 1 should read lines from the user from stdin and save each line in a new node of the linked list 
	//we use the do while loop for this purpose and also print out the necessary thread, PID and stuffs for users to see 
	char* buffer;
	do
	{
	    printf("Enter a line: ");
		fgets(buffer,100,stdin);
		if(buffer== NULL)
		{
			is_reading_complete = true;
		}
		if(is_reading_complete == false)
		{
			    if (hours < 12)   		// before midday
				{
					printf("Logindex %ld, thread %ld, PID %d, %02d/%02d/%d, %02d:%02d:%02d am.\n", Logindex, me, PID, day, month, year , hours, minutes, seconds);
					printList();
					Logindex++;
				}
				else					// after midday
				{
					printf("Logindex %ld, thread %ld, PID %d, %02d/%02d/%d, %02d:%02d:%02d pm.\n", Logindex, me, PID, day, month, year , hours - 12, minutes, seconds);
					printList();
					Logindex++;
				}
				insertFirst(buffer);
		}
	
	}while(fgets(buffer,100,stdin) != NULL);

	//Each message printed to stdout should be prefixed with a log index, thread index, PID, and date and time
	if (hours < 12)   		// before midday
	{
		printf("Logindex %ld, thread %ld, PID %d, %02d/%02d/%d, %02d:%02d:%02d am.\n", Logindex, me, PID, day, month, year , hours, minutes, seconds);
		printList();
		Logindex++;
	}
	else					// after midday
	{
		printf("Logindex %ld, thread %ld, PID %d, %02d/%02d/%d, %02d:%02d:%02d pm.\n", Logindex, me, PID, day, month, year , hours - 12, minutes, seconds);
		printList();
		Logindex++;

	}
  	pthread_mutex_unlock(&tlock1);  // critical section ends
	
	if (p!=NULL && p->creator==me)    
 	{
 		printf("This is thread %ld and I created the THREADDATA %p\n",me,p); 
 	    if (hours < 12)   		// before midday
		{
			printf("Logindex %ld, thread %ld, PID %d, %02d/%02d/%d, %02d:%02d:%02d am.\n", Logindex, me, PID, day, month, year , hours, minutes, seconds);
			printList();
			Logindex++;
		}
		else					// after midday
		{
			printf("Logindex %ld, thread %ld, PID %d, %02d/%02d/%d, %02d:%02d:%02d pm.\n", Logindex, me, PID, day, month, year , hours - 12, minutes, seconds);
			printList();
			Logindex++;
		}
 	}
 	else   
 	{
 		printf("This is thread %ld and I can access the THREADDATA %p\n",me,p);
 		if (hours < 12)   		// before midday
		{
			printf("Logindex %ld, thread %ld, PID %d, %02d/%02d/%d, %02d:%02d:%02d am.\n", Logindex, me, PID, day, month, year , hours, minutes, seconds);
			printList();
			Logindex++;
		}
		else					// after midday
		{
			printf("Logindex %ld, thread %ld, PID %d, %02d/%02d/%d, %02d:%02d:%02d pm.\n", Logindex, me, PID, day, month, year , hours - 12, minutes, seconds);
			printList();
			Logindex++;
		}
 	}

	// TODO use mutex to make this a start of a critical section   
 	pthread_mutex_lock(&tlock2);

  	if (p!=NULL && p->creator==me)    
  	{
  		printf("This is thread %ld and I didnot touch THREADDATA\n",me);
  		if (hours < 12)   		// before midday
		{
			printf("Logindex %ld, thread %ld, PID %d, %02d/%02d/%d, %02d:%02d:%02d am.\n", Logindex, me, PID, day, month, year , hours, minutes, seconds);
			printList();
			Logindex++;
		}
		else					// after midday
		{
			printf("Logindex %ld, thread %ld, PID %d, %02d/%02d/%d, %02d:%02d:%02d pm.\n", Logindex, me, PID, day, month, year , hours - 12, minutes, seconds);
			printList();
			Logindex++;
		} 
  	}
   	else
   	{  
  		/**TODO Free the THREADATA object. Freeing should be done by the other thread from the one that created it.  
   		* See how the THREADDATA was created for an example of how this is done.   */   
  		free(p);									//second thread deallocates it
  		printf("This is thread %ld and I deleted the THREADDATA\n",me);  
  		if (hours < 12)   		// before midday
		{
			printf("Logindex %ld, thread %ld, PID %d, %02d/%02d/%d, %02d:%02d:%02d am.\n", Logindex, me, PID, day, month, year , hours, minutes, seconds);
			printList();
			Logindex++;
		}
		else					// after midday
		{
			printf("Logindex %ld, thread %ld, PID %d, %02d/%02d/%d, %02d:%02d:%02d pm.\n", Logindex, me, PID, day, month, year , hours - 12, minutes, seconds);
			printList();
			Logindex++;
		}
  	}  

  	// T2 could do this in a while loop where it checks the is_reading_complete flag to know when to stop.
    while(is_reading_complete == true)
    {
  		printf("This is thread %ld and I will print out the data in the node!\n",me); 
  	   if (hours < 12)   		// before midday
	{
		printf("Logindex %ld, thread %ld, PID %d, %02d/%02d/%d, %02d:%02d:%02d am.\n", Logindex, me, PID, day, month, year , hours, minutes, seconds);
		printList();
		Logindex++;
	}
	else					// after midday
	{
		printf("Logindex %ld, thread %ld, PID %d, %02d/%02d/%d, %02d:%02d:%02d pm.\n", Logindex, me, PID, day, month, year , hours - 12, minutes, seconds);
		printList();
		Logindex++;
	}
    	printList();//print data in the node just added 
    }

	//Each message printed to stdout should be prefixed with a log index, thread index, PID, and date and time
    if (hours < 12)   		// before midday
	{
		printf("Logindex %ld, thread %ld, PID %d, %02d/%02d/%d, %02d:%02d:%02d am.\n", Logindex, me, PID, day, month, year , hours, minutes, seconds);
		printList();
		Logindex++;
	}
	else					// after midday
	{
		printf("Logindex %ld, thread %ld, PID %d, %02d/%02d/%d, %02d:%02d:%02d pm.\n", Logindex, me, PID, day, month, year , hours - 12, minutes, seconds);
		printList();
		Logindex++;
	}
    // TODO critical section ends
   	pthread_mutex_unlock(&tlock2);
	
	printf("This is thread %ld and now I will exit the thread!\n",me); 


	if (hours < 12)   		// before midday
	{
		printf("Logindex %ld, thread %ld, PID %d, %02d/%02d/%d, %02d:%02d:%02d am.\n", Logindex, me, PID, day, month, year , hours, minutes, seconds);
		printList();
		Logindex++;
	}
	else					// after midday
	{
		printf("Logindex %ld, thread %ld, PID %d, %02d/%02d/%d, %02d:%02d:%02d pm.\n", Logindex, me, PID, day, month, year , hours - 12, minutes, seconds);
		printList();
		Logindex++;
	}
  	pthread_exit(NULL);
	return NULL;
}



FILE *file;
pthread_t tid1, tid2;

//main method to evolt the command, try to read in the file if successful
//main method willing calling the two threads and join methods to wait for each thread to finish running before another thread take place.
int main(int argc, char **argv)
{
	//struct THREADDATA *my_threat_struct;

	//run the first thread
  	printf("Create first thread\n");
  	pthread_create(&tid1,NULL,thread_runner,NULL);

  	//run the second thread
	printf("Create second thread\n"); 
	pthread_create(&tid2,NULL,thread_runner,NULL);

	//Allows the calling thread to wait for the ending of the target thread.
  	printf("Wait for first thread to exit\n");  
  	pthread_join(tid1,NULL);
  	printf("First thread exited\n");

	//Allows the calling thread to wait for the ending of the target thread.
 	 printf("wait for second thread to exit"); 
 	 pthread_join(tid2,NULL);  
 	 printf("second thread exited");  
 	 exit(0);

    return 0;
} //end main


