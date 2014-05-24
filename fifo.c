#include<stdio.h>
#include "fifo.h"
/* Queue structure */
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

Node* queue_get()
{
  Node* old;
  
  if(queueIn == queueOut)
    {
      printf("queue is empty");
      return NULL; /* queue Empty - nothing to get*/
    }

  old = queue[queueOut];

  print_node(old);

  queueOut = (queueOut + 1) % QUEUE_SIZE;

  return old; // No errors
}

void print_size()
{
  printf("queue_size:%d\n", queueIn - queueOut);
}

int queue_length()
{
  int size = queueIn - queueOut;
  return size;
}

void print_head()
{
  printf("Cabeça\n");
  print_node(queue[queueOut]);
}
