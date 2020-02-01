/*test machine: CSELAB_machine_name * date: 11/13/19
* name: Ansh Sikka
* x500: sikka008  */

/*
header.h, header for all source files
it will: 
- structure definition
- global variable, lock declaration (extern)
- function declarations
*/
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>


#ifndef _HEADER_H_
#define _HEADER_H_

// header here..
extern int consumers;
extern char* filename;
extern int queue_size;
extern int logger;
extern int bounded;
extern FILE *text_file;
extern FILE *final_result;
extern FILE *log_result;
extern int saved_stdout;



typedef struct Node {
    struct Node * next;
    char line[1024];
    int line_num;
} node_t;

struct shared_queue {
    node_t* head; // the consumer gets data from the head
    pthread_cond_t* cond; // condition variable
    pthread_cond_t* buffer_size_cond; // condition variable for fixed buffer size
    pthread_mutex_t* mutex; // the mutex lock
    int count;
} queue_t;

typedef struct letter_counts {
    char letter;
    int count;
} counts_t;



extern struct letter_counts* lc;
extern struct shared_queue* sq;


void init_counts (counts_t counts[]);

int read_file(char *file_path);

void *produce(void * queue);

void *consume(void * queue);








#endif
