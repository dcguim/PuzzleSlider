#include<stdio.h>
#include "fifo.h"
/* Queue structure */
// Each graph has no more than 9^9 possible configurations which is 387,420489 < INT_MAX = 2,147,483647
#define QUEUE_SIZE (MAX_GRAPH_SIZE + 1)
Node* queue[QUEUE_SIZE]; 
static int queueIn, queueOut;

void queue_init(void)
{
    queueIn = 0;
    queueOut = 0;
}

int queue_put(Node* new)
{
    if(queueIn == (( queueOut - 1 + QUEUE_SIZE) % QUEUE_SIZE))
    {
      printf("qI:%d",queueIn);
      printf ("queue is full\n");
      fflush(stdout);
      return -1; /* Queue Full*/
    }

    queue[queueIn] = new;

    queueIn = (queueIn + 1) % QUEUE_SIZE;

    return 0; // No errors
}

int queue_get(Node *old)
{
    if(queueIn == queueOut)
    {
      printf("queue is empty");
        return -1; /* queue Empty - nothing to get*/
    }

    old = queue[queueOut];

    queueOut = (queueOut + 1) % QUEUE_SIZE;

    return 0; // No errors
}

void print_size()
{
  printf("%d", queueIn - queueOut);
}
