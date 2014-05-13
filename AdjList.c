#include"AdjList.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
/*
TODO
Implementar uma mais um nível de abstraçao que aponta
para os nós existentes.
*/
static Node* head;

Node* create_list( int startPoint[] )
{
  int i;

  head = (Node*) malloc(sizeof(Node));
  if (head == NULL)
    {
      printf ("Could not allocate head!\n");
      exit (0);
    }
  
  for (i = 0; i < CONFIG_SIZE; i++)
    {
      head->config[i] = startPoint[i];
      if (startPoint[i] == 0)
	head->missPiecePos = i;
    }

  head->neighbor = NULL;
  
  return head;
}

void print_node (Node* n)
{
  int i;

  for (i = 0; i < CONFIG_SIZE; i++)
    {
      printf ("[%d]: %d\n",i, n->config[i]);
    }
  printf("hole:[%d]\n",n->missPiecePos);
}

void which_neighbors (Node* curr, int neighbors[])
{
  int i;
  // Init neighbors with -1
  for (i = 0; i < MAX_NEIGHBORS; i++)
    neighbors[i] = -1;

  // There is a left-side neighbor
  if ((curr->missPiecePos % 3) == 2)
    neighbors[1] = curr->missPiecePos -1;
  // There is a (left and right)side neighbor
  else if ((curr->missPiecePos % 3) == 1)
    {
     neighbors[3] = curr->missPiecePos + 1;
     neighbors[1] = curr->missPiecePos - 1;
    }
  // Exists a right-side neighbor
  else //((curr->missPiecePos % 36) == 0)
    neighbors[3] = curr->missPiecePos + 1;

  // Exists a lower neighbor
  if ((curr->missPiecePos / 3) == 0)
    neighbors[2] = curr->missPiecePos + 3;
  // Exists a upper and lower neighbor
  else if ((curr->missPiecePos / 3) == 1)
    {
     neighbors[0] = curr->missPiecePos - 3;
     neighbors[2] = curr->missPiecePos + 3;
    }
  // Exists a upper neighbor
  else //((curr->missPiecePos / 3) == 2)
    neighbors[0] = curr->missPiecePos - 3;
}


void create_neighbor ( Node* curr, const int config[] )
{
  int i;
  Node* new,* ant,* aft;
   
  new = (Node*) malloc(sizeof(Node));
  if (new == NULL)
    {
      printf("the neighbor could no be allocated");
      exit (0);
    }

  // Find the last node created
  aft = curr;
  while (aft != NULL)
    {
      ant = aft;
      aft = aft->neighbor;
    }

  // Initialize new`s fields
  ant->neighbor = new;
  new->neighbor = NULL;
  new->visited = FALSE;

  // new`s config fill
  for (i = 0; i < CONFIG_SIZE; i++)
    {
      new->config[i] = config[i];
      // Indicate where the hole is
      if (config[i] == 0)
	new->missPiecePos = i;
    }
}

void free_list (Node* begin)
{
  int i = 0;
  Node* aft;
  Node* ant;
  aft = begin;
  
  while (aft != NULL)
    {
      ant = aft;
      aft = aft->neighbor;
      free(ant);
      i++;
    }
  printf ("list freed! (%d nodes found) \n", i);
}
