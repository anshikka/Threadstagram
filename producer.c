/*test machine: CSELAB_machine_name * date: 11/13/19
* name: Ansh Sikka
* x500: sikka008  */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"
// pthread.h included in header.h

int read_file(char *file_path)
{
    text_file = fopen(file_path, "r"); // attempt to open file
    if (text_file == NULL)
    {
        perror("File corrupted or not found.\n");
        exit(1);
    }
    return 0; // on success
}

void *produce(void *queue)
{
    if (logger)
    {
        printf("producer\n");
    }
    char data[1024]; // temporary data on stack allocated for transferring data from file to node
    int line_num = 1;
    while (fgets(data, 1024, text_file))
    {                                  // while not end of file
        pthread_mutex_lock(sq->mutex); // lock producer mutex
        if (logger)
        {
            printf("producer: %d\n", line_num);
        }
        /* Extra Credit
         * If it's a bounded buffer, it waits based on if the queue size is at MAX
         */
        if (bounded)
        {
            while (sq->count == queue_size)
            {
                pthread_cond_wait(sq->buffer_size_cond, sq->mutex); // wait until queue has capacity
            }
        }
        // Case 1: Head is empty. Put data in head
        if (sq->head == NULL)
        {
            sq->head->line_num = line_num;
            strcpy(sq->head->line, data);
            sq->head->next = NULL;
            // Case 2: Head is not empty, move to next open node.
        }
        else
        {
            node_t *cur = sq->head;
            while (cur->next != NULL)
            {
                cur = cur->next;
            }
            // on the last node
            cur->next = (struct Node *)malloc(sizeof(node_t));
            cur->next->line_num = line_num;
            strcpy(cur->next->line, data);
            cur->next->next = NULL;
        }
        sq->count++;
        line_num++;
        pthread_cond_signal(sq->cond);   // finished adding to list
        pthread_mutex_unlock(sq->mutex); // unlock for consumers to perform ops
    }

    /*
     * For the remaining consumers, 
     * send EOF notifications
     */
    for (int k = 0; k < consumers; k++)
    {
        pthread_mutex_lock(sq->mutex);
        // Case 1: Head is empty. Put data in head
        if (sq->head == NULL)
        {
            strcpy(sq->head->line, "-1"); // "-1" indicates that it is EOF
            sq->head->line_num = -1;
            sq->head->next = NULL;
            // Case 2: Head is not empty, move to next open node.
        }
        else
        {
            node_t *cur = sq->head;
            while (cur->next != NULL)
            {
                cur = cur->next;
            }
            // on the last node
            cur->next = (struct Node *)malloc(sizeof(node_t));
            strcpy(cur->next->line, "-1");
            sq->head->line_num = -1;
            cur->next->next = NULL; // at the end
        }
        sq->count++;
        pthread_cond_broadcast(sq->cond); // finished adding to list
        pthread_mutex_unlock(sq->mutex);  // unlock for other threads to perform ops
    }

    return NULL;
}
