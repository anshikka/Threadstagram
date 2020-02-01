/*test machine: CSELAB_machine_name * date: 11/13/19
* name: Ansh Sikka
* x500: sikka008  */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "header.h"
// pthread.h included in header.h

// for each word, get the first letter and increase count
void process_first_letter(char letter)
{
    for (int i = 0; i < 26; i++)
    {
        if (lc[i].letter == tolower(letter))
        {
            lc[i].count += 1;
            break;
        }
    }
}

int synchronize(char *data)
{
    // handle char* processing here
    int i = 0;
    int new_word = 1; // indicates that next char that is coming is the beginning of a new word
    while (data[i] != '\0')
    {
        if ((data[i] >= 'a' && data[i] <= 'z') || (data[i] >= 'A' && data[i] <= 'Z'))
        {
            if (new_word)
            { // for each token
                process_first_letter(data[i]);
                new_word = 0;
            }
        }
        else
        { // if there is any type of space character
            new_word = 1;
        }
        i++;
    }

    return 0;
}

void *consume(void *consumer_id)
{
    long int con_id = (long int) consumer_id;
    if (logger)
    {
        printf("consumer %ld\n", con_id);
    }
    while (strcmp(sq->head->line, "-1") != 0)
    {                                  // continue operations until EOF is reached
        pthread_mutex_lock(sq->mutex); // lock consumer mutex
        while (sq->count == 0)
        { // if the queue is empty, wait on producer to add to the queue
            pthread_cond_wait(sq->cond, sq->mutex);
        }
        if (logger)
        {
            printf("consumer %ld: %d\n", con_id, sq->head->line_num);
        }
        char data[1024];
        strcpy(data, sq->head->line); // get the data from the head
        // CASE 1: Head is the only node in the queue, set it to NULL
        if (sq->head->next == NULL)
        {
            sq->head = NULL;
        }
        // CASE 2: Set head to the next node in the queue (discards current head)
        else
        {
            sq->head = sq->head->next;
        }
        sq->count--;
        /* Extra Credit
         * If it's a bounded buffer, it signals based on if the queue size is below the maximum specified
         */
        if (bounded && sq->count < queue_size)
        {
            pthread_cond_signal(sq->buffer_size_cond);
        }
        synchronize(data);               // process the line of data and extract first letters from each word
        pthread_mutex_unlock(sq->mutex); // unlock for producer/other consumers to perform ops
    }
    return NULL;
}
