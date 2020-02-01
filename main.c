/*test machine: CSELAB_machine_name * date: 11/13/19
* name: Ansh Sikka
* x500: sikka008  */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "header.h"
// pthread.h included in header.h

/* 
 * Initialize data structures
 * Initialize mutexes, conditional variables, head nodes, and counts
 */
int init_data_structures()
{
    sq = (struct shared_queue *)malloc(sizeof(struct shared_queue));         // shared queue
    sq->mutex = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));          // mutex for consumer to wait on producer
    sq->cond = (pthread_cond_t *)malloc(sizeof(pthread_cond_t));             // conditional variable for consumer waiting on producer
    sq->buffer_size_cond = (pthread_cond_t *)malloc(sizeof(pthread_cond_t)); // EXTRA CREDIT: conditional variable on producer waiting until consumer empties queue to under maximum capacity
    sq->head = (node_t *)malloc(sizeof(node_t));                             // head node
    sq->count = 0;                                                           // queue count
    /*  
    * Initialize thread conditions and mutexes
    */
    pthread_cond_init(sq->cond, NULL);
    pthread_cond_init(sq->buffer_size_cond, NULL);
    pthread_mutex_init(sq->mutex, NULL);
    /*
     * Initialize data structure to count letters 
     */
    lc = (struct letter_counts *)malloc(26 * sizeof(struct letter_counts));
    init_counts(lc);
    text_file = (FILE *)malloc(sizeof(FILE)); // file being loaded in
    return 0;
}

// initialize struct with letters and counts = 0
void init_counts(counts_t *counts)
{
    for (int i = 0; i < 26; i++)
    {
        counts[i].letter = i + 'a';
        counts[i].count = 0;
    }
}

int init_threads(pthread_t *cond_pool, pthread_t producer)
{
    /*
    * Pass produce function into producer
    */
    pthread_create(&producer, NULL, produce, NULL);
    for (int i = 0; i < consumers; i++)
    {
        /*
        * Pass consume functon into each consumer
        * Pass consumer id into each consumer
        */
        int id = i + 1;
        pthread_create(&cond_pool[i], NULL, consume, (void *) (size_t) id);
    }
    return 0;
}

int join_threads(pthread_t *cond_pool, pthread_t producer)
{
    for (int i = 0; i < consumers; ++i)
    {
        pthread_join(cond_pool[i], NULL); // wait for consumer threads
    }
    pthread_join(producer, NULL); // wait for producer thread if not finished
    return 0;
}

int finalize()
{
    final_result = fopen("result.txt", "a"); // create new file: result.txt for appendation
    for (int i = 0; i < 26; i++)
    {
        fprintf(final_result, "%c: %d\n", lc[i].letter, lc[i].count); // print each line to file
    }
    return 0;
}

int main(int argc, char *argv[])
{
    // Without logging or restricted buffer size
    if (argc == 3)
    {
        logger = 0;
        consumers = atoi(argv[1]);
        filename = argv[2];
        pthread_t cond_pool[consumers];
        pthread_t producer;
        init_data_structures();
        read_file(filename);
        init_threads(cond_pool, producer);
        join_threads(cond_pool, producer);
        finalize();
    }
    // Options
    else if (argc == 4)
    {
        // With logging only
        if (strcmp("-p", argv[3]) == 0)
        {
            logger = 1;
            bounded = 0;
            log_result = fopen("log.txt", "a"); // open log file for appendation
            int log_file_num = fileno(log_result);
            dup2(log_file_num, STDOUT_FILENO); // redirect stdout to log.txt file number
            consumers = atoi(argv[1]);
            filename = argv[2];
            pthread_t cond_pool[consumers];
            pthread_t producer;
            init_data_structures();
            read_file(filename);
            init_threads(cond_pool, producer);
            join_threads(cond_pool, producer);
            finalize();
        }
    }
    // With restricted buffer size
    else if (argc == 5)
    {
        // restricted buffer size AND logging
        if (strcmp("-bp", argv[3]) == 0)
        {
            logger = 1;
            bounded = 1;
            log_result = fopen("log.txt", "a"); // open log file for appendation
            int log_file_num = fileno(log_result);
            dup2(log_file_num, STDOUT_FILENO); // redirect stdout to log.txt file number
            consumers = atoi(argv[1]);
            filename = argv[2];
            queue_size = atoi(argv[4]);
            pthread_t cond_pool[consumers];
            pthread_t producer;
            init_data_structures();
            read_file(filename);
            init_threads(cond_pool, producer);
            join_threads(cond_pool, producer);
            finalize();
        }
        // restricted buffer size ONLY
        else if (strcmp("-b", argv[3]) == 0)
        {
            logger = 0;
            bounded = 1;
            consumers = atoi(argv[1]);
            filename = argv[2];
            queue_size = atoi(argv[4]);
            pthread_t cond_pool[consumers];
            pthread_t producer;
            init_data_structures();
            read_file(filename);
            init_threads(cond_pool, producer);
            join_threads(cond_pool, producer);
            finalize();
        }
    }

    else if (argc < 3)
    {
        perror("Too little number of arguments!\n");
        exit(1);
    }
    else
    {
        perror("Too many arguments!\n");
        exit(1);
    }
    return 0;
}
