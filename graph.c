#include<stdlib.h>
#include<stdio.h>
#include<limits.h>
#include "tree.h"

static Node* graph[MAX_GRAPH_SIZE];
unsigned int it;
static Node* head;

// Closest key index found from the input key 
static int closestKey;
static int foundKey;
static int hasCreatedHead;
static Tree* DFS_Trees[2];

void create_graph(int config[]);
void init_search();
Node* insert_node (Node* parent,int conf[]);
int binary_search_mod (int begin, int end, int key);
void create_recursive(Node* parent);


void init_search()
{
  closestKey = -1;
  foundKey = -1;
}

void create_iterative(Node* parent)
{
  Node* new;
  Node* father;
  int i,j;
  int nBors[MAX_NEIGHBORS];
  int nConfig[CONFIG_SIZE];
  
  // Initializes the queue and put the start node into it
  queue_init();  
  queue_put(parent);
  
  while ( (queue_length() != 0))
    {
      print_head(); 
      
      father = queue_get();
      // Get the nightbors to be created from the parent node
      which_neighbors(father,nBors);
      
      // Create the necessary nodes  
      for (i = 0; i< MAX_NEIGHBORS; i++)
	{
	  // if neighbor should be created
	  if (nBors[i] != -1)
	    {
	      // Copy the previous configuration
	      for (j = 0; j< CONFIG_SIZE; j++) 
		nConfig[j] = father->config[j];

	      // Swap the hole with the nBors[i]
	      nConfig[father->missPiecePos] = nConfig[nBors[i]];
	      nConfig[nBors[i]] = 0;
	      	      	      
	      printf("Config to be inserted in the stack(Son)\n");
	      for (j = 0; j < CONFIG_SIZE; j++)
		{
		  printf ("[%d]:%d", j, nConfig[j]);
		  if (((j+1)%3) == 0)
		    printf("\n");
		}
	      	      
	      new = insert_node(father, nConfig);
	      
	      
	      if (new != NULL)
		{// Stack the new node up
		  print_node(new);
		  queue_put(new);
		  print_head();
		  print_size();
		}
	    }
	}
    }
  //print_node(parent);
}

void create_graph(int config[]){
  
  Node* new = NULL;

  head = insert_node(new,config);

  head->visited = TRUE;  

  create_iterative(head);
}

// void DFS_visit(Node* node)
// {
//   int i;
//   node->visited=TRUE;
//   for (i=0; i<node->noNeighbor; i++){
//     if(node->neighbor[i]->visited == FALSE){
//       queue_put(node->neighbor[i]);
//       DFS_visit(node->neighbor[i]);
//   }
// }
// 
// void DFS()
// {
//   int i;
//   for (i =0; i< it; i++){
//     if(graph[i]->visited == FALSE){
//       DFS_visit(graph[i]);
//   }
// }

void DFS_visit(Node* node)
{
  int i;
  node->visited=TRUE;
  for (i=0; i<node->noNeighbor; i++){
    if(node->neighbor[i]->visited == FALSE){
      queue_put(node->neighbor[i]);
      DFS_visit(node->neighbor[i]);
    }  
  }
}

/*
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
*/

Node* insert_node (Node* parent,int conf[] )
{
  Node* aux,*a,*new;
  int tmp,i,key,pos;
  
  // Head was created previously so it
  // just needs to be inserted into the graph
  if (hasCreatedHead == -1)
    {
      new = create_head(conf);
      graph[0] = new;      
      hasCreatedHead = 1;
      return new;
    }
  
  key = generate_key(conf);

  init_search();
  binary_search_mod (0,it,key);
  
   // node was not found
  if (foundKey == -1 && closestKey != -1)
    {
      tmp = closestKey;

      // This piece of code make sure the tmp iterator is pointing to the
      // greater smaller key when compared to the key to be inserted 
      if (graph[tmp]->key > key)
	{
	  // This should iterate 1 maybe 2 times
	  while (graph[tmp]->key > key)
	    {
	      tmp--;
	      if (tmp < 0)
		  break;
		
	    }	 
	}
      else if (graph[tmp]->key < key)
	{
	  // 1 or 2 times too.
	  while (graph[tmp]->key < key)
	    {
	      if (graph[tmp+1] == NULL)
		  break; 
	      // if the next key is already greater than ok, it doesnt need to search anymore
	      if (graph[tmp + 1]->key > key || graph[tmp+1] == NULL)
		break;
	      else
		tmp++;
	    }
	}

      // The size of graph must be increased by one
      it = it + 1;
      printf(">it = %d<\n", it);
      // There are at least two nodes to reach graph[it]
      if (tmp + 1 < it)
	{
	  // I want to leave a hole for the node to be inserted, this explain the jump
	  tmp = tmp + 2;
	  // Open the hole(at tmp - 1) and push forward once i.e swap
	  aux = graph[tmp];
	  pos = tmp - 1;
	  graph[tmp] = graph[tmp - 1];
	  tmp++;
	  i = tmp;
	  // push to the right every node with greater key once
	  while (i < it)
	    {
	      a = graph[i];
	      graph[i] = aux;
	      aux = a;
	      i++;
	    }
	  // For the last item is just need  put the aux into it
	  graph[i] = aux;
	}
      else if (tmp + 1 == it) // There is exactly one node to reach graph[it]
	{
	  tmp++;
	  aux = graph[tmp];
	  pos = tmp;
	  graph[it] = aux;
	}
      else// new node must be set to the last position
	{
	  pos = it;
	}
      printf(">pos = %d<\n",pos );
      new = create_neighbor(parent, conf);
      graph[pos] = new;

      return new;  
    }
  
  else if (foundKey >= 0)    
    printf("\nFound! [%d]\n", foundKey);
    
  return NULL;
}

int binary_search_mod (int begin, int end, int key)
{
  int pivot;
  while (begin <= end)
    {
      pivot = (begin+end)/2;  
      if (key == graph[pivot]->key)
	{
	  foundKey = pivot;
	  break;
	}
      else if (key < graph[pivot]->key)
	{
	  end = pivot - 1;
	}
      else
	{
	  begin = pivot + 1;
	}
    }
  // Stop condition
  if (begin > end)
    {

      if (end < 0)
	{
	  closestKey = 0;
	  return 0;
	}
      else if ( begin > it)
	{
	  closestKey = it;
	  return 0;
	}
      closestKey = end;
    }
  return 1;
}

int main (void)
{
  int cOne[9] = {0,1,2,3,4,5,6,7,8};

  int cTwo[9] = {3,1,2,0,4,5,6,7,8};
  int cThree[9] = {2,1,5,3,4,0,6,7,8};
  int cFour[9] = {1,0,2,3,4,5,6,7,8};

  Node* n1 = create_head(cOne);
  Node* n2 = create_head(cTwo);
  Node* n3 = create_head(cThree);
  Node* n4 = create_head(cFour);
  Tree* t1 = create_tree(n1);
  add_child(t1,n1,n2);
  add_child(t1,n1,n4);
  add_child(t1,n4,n3);
  
  printf("Nodes: %d Edges: %d\n",t1->nNodes,t1->nEdges);
  print_tree(t1->root,0);
   
  // hasCreatedHead = -1;
  // create_graph(cOne);
  
  // DFS();
  
  // free_list(graph, it);

  return 0;
}
