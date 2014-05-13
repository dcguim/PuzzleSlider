#include"AdjList.h"
#include<stdlib.h>

void DFS_algorithm(Node* start)
{
  int i,j;
  int nBors[MAX_NEIGHBORS];
  int nConfig[CONFIG_SIZE];
  Node* aux;
  
  start->visited = TRUE;
  // Return in nBors the nodes to be created
  which_neighbors(start,nBors);

  // Create the necessary nodes
  for (i = 0; i< MAX_NEIGHBORS; i++)
    {
      if (nBors[i] != -1)
	{
	  for (j = 0; j< CONFIG_SIZE; j++) 
	    nConfig[j] = start->config[j];
	  
	  // Swap the hole with the nBors[i]
	  nConfig[start->missPiecePos] = nConfig[nBors[i]];
	  nConfig[nBors[i]] = 0;
	 
	  create_neighbor(start, nConfig);
	}
    }

  aux = start;
  while (aux != NULL)
    {
      print_node(aux);
      aux = aux->neighbor;
    }
}

int main( void )
{
  int startPoint[9] = {3,2,0,5,1,7,6,4,8};
  
  Node* head = create_list(startPoint);
  DFS_algorithm(head);

  free_list(head);
  return 0;
}
