#include"fifo.h"
#include<stdlib.h>
#include<stdio.h>
#include<limits.h>

static Node* graph[MAX_GRAPH_SIZE];
static unsigned int it;

void DFS_algorithm(Node* start)
{
  int i,j;
  int nBors[MAX_NEIGHBORS];
  int nConfig[CONFIG_SIZE];
  Node* aux;

  print_size();
  queue_get(start);
  printf("Analisando ...\n");
  print_node(start);
  
  start->visited = TRUE;
  // Return in nBors the nodes to be created
  which_neighbors(start,nBors);
  
  // Create the necessary nodes
  for (i = 0; i< MAX_NEIGHBORS; i++)
    {
      // if neighbor should be created
      if (nBors[i] != -1)
	{
	  for (j = 0; j< CONFIG_SIZE; j++) 
	    nConfig[j] = start->config[j];

	  // Swap the hole with the nBors[i]
	  nConfig[start->missPiecePos] = nConfig[nBors[i]];
	  nConfig[nBors[i]] = 0;

	  aux = create_neighbor(start, nConfig);
	  
	  print_node(aux);
	  graph[it] = aux;
	  it++;
	}
    }
}

/*
TODO
Implementar uma mais um nível de abstraçao que aponta
para os nós existentes.
*/

int main( void )
{
  int startPoint[9] = {3,2,0,5,1,7,6,4,8};
  it = 0;
  Node* head = create_head(startPoint);
  
  graph[it] = head;
  it++;

  queue_init();
  queue_put(head);
  DFS_algorithm(head);

  print_node(head);
  free_list(graph, it);
  return 0;
}
