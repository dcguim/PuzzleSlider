#include"AdjList.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

/************** Private Prototypes  ******************/

Node* create_node(const int nodeConfig[] );

/*****************************************************/

/***** Static Variables ****/
static Node* head;
static int edges;
/**************************/

Node* create_node(const int nodeConfig[] )
{
  int i;
  bool hasHole = FALSE;
  Node* newNode = (Node*) malloc(sizeof(Node));
  
  // Fill the config, missPiece
  for (i = 0; i < CONFIG_SIZE; i++)
    {
      newNode->config[i] = nodeConfig[i];
      if (nodeConfig[i] == 0)
	{
	  hasHole = 1;
	  newNode->missPiecePos = i;
	}
    }
  // The neightbor created has no neighbors yet
  for (i=0; i < MAX_NEIGHBORS ;i++){
    newNode->neighbor[i] = NULL;
  }  

  // The node created has no childs yet
  for (i=0; i<MAX_CHILDREN; i++)
    newNode->child[i] = NULL;

  // It wasn`t visited and the number of neighbors and child is zero
  newNode->time = 0;
  newNode->dfs_visited = FALSE;
  newNode->bfs_visited = FALSE;
  newNode->noNeighbor = 0;
  newNode->noChild = 0;
  newNode->key = generate_key(nodeConfig);
  
  return newNode;
}

void print_node (Node* n)
{
  int i;

  for (i = 0; i < CONFIG_SIZE; i++)
    {      
      printf ("[%d]:%d", i, n->config[i]);
      if (((i+1)%3) == 0)
	printf("\n");
    }
  for (i = 0; i < MAX_NEIGHBORS; i++)
    {
      if (n->neighbor[i] == NULL)
	printf("neighbor[%d] = NULL\n", i);
    }
  printf("Number of neightbors :%d\n",n->noNeighbor);
  printf("hole:[%d]\n",n->missPiecePos);
  printf("noChild: %d\n",n->noChild);
  printf("time: %d\n", n->time);
  printf("key: %d\n",n->key);
}

void which_neighbors (Node* curr, int neighbors[])
{
  int i;
  // Init neighbors with -1
  for (i = 0; i < MAX_NEIGHBORS; i++)
    neighbors[i] = -1;

  fflush(stdout);
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
  
  fflush(stdout);
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

Node* create_head ( int config[] )
{
  Node* newNode = create_node( config );

  // Keeps the start-position node of the game
  head = newNode;

  edges = 0;

  return newNode;
}

int generate_key(const int config[])
{
  int i,key = 0,pot = 1;
  for (i = 0; i < CONFIG_SIZE; i++)
    {
      key += (config[i] + 1)*pot;
      pot = pot*10;
    }
  return key;
}

Node* create_neighbor ( Node* curr, const int config[] )
{
  Node* newNode;
  
  newNode = create_node( config );
  curr->neighbor[curr->noNeighbor] = newNode;
  curr->noNeighbor++;
 
  return newNode;
}

void free_list ( Node** graph, int n )
{
  int i;
  //Node* aux;

  if (n == 0)
    {
      free(graph[0]);
      return;
    }
  for (i = 0; i <= n; i++ )
    {
      if (graph[i] != NULL)
	{
	  free(graph[i]);
	}

    }
  
  printf ("list freed! (%d nodes found) \n", i);
}

void add_child(Node* parent, Node* child)
{
  if(parent->noChild >= MAX_CHILDREN)
    return;
  parent->child[parent->noChild] = child;
  child->parent = parent;
  parent->noChild++;
  edges+=1;
}
