/*test machine: CSELAB_machine_name * date: 11/13/19
* name: Ansh Sikka
* x500: sikka008  */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"
// pthread.h included in header.h

// implement shared queue, final histogram here..

// unbounded-buffer shared queue

struct letter_counts *lc;
struct shared_queue *sq;

int consumers;      // number of consumers
char *filename;     // file name of text file
int queue_size;     // specified queue size (extra credit)
int logger;         // wheter or not it should be logged
int bounded;        // whether or not its bounded
FILE *text_file;    // text file opened for processing
FILE *final_result; // final result file pointer
FILE *log_result;   // log result file pointer
int saved_stdout;   // saved STDOUT file number