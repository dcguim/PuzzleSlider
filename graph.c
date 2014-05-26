#include<stdlib.h>
#include<stdio.h>
#include<limits.h>
#include "tree.h"
#define NO_CCOMP 2

static Node* graph[NO_CCOMP][MAX_GRAPH_SIZE];
static int cCompCreate;
unsigned int it;
static Node* head[NO_CCOMP];
static int cCompSize;
static int nEdges[NO_CCOMP];
static int GraphEdges = 0;

// Closest key index found from the input key 
static int closestKey;
static int foundKey;
static int hasCreatedHead;

void create_graph(int config[]);
void init_search();
Node* insert_node (Node* parent,int conf[]);
int binary_search_mod (int begin, int end, int key);
void create_iterative(Node* parent);

void init_create_graph(int cComp)
{
  hasCreatedHead = -1;
  cCompCreate = cComp - 1;
  it = 0;   
}

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
  int edges =0;
  
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
        // Insert the edges to the neighbors
      edges += nBors[i];
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
	      else // There is a found key, just need set in the neighbors
        {
          if  (father->noNeighbor > 3)
          {
            printf("Tentou inserir mais do que 4 vizinhos para um mesmo nó! ");
            fflush(stdout);
            exit(0);
          }
          father->neighbor[father->noNeighbor] = graph[cCompCreate][foundKey];
          father->noNeighbor++;
          printf("father:\n");
          print_node(father);
        }
      }
    }
  }
  GraphEdges += edges/2;
  //print_node(parent);
}

void create_graph(int config[]){
  
  Node* new = NULL;

  head[cCompCreate] = insert_node(new,config);

  nEdges[cCompCreate] = 0;

  create_iterative(head[cCompCreate]);
}


void DFS_visit(Node* node, int compIndex)
{
  // printf("node visited - %d with %d neighbors\n", node->dfs_visited,node->noNeighbor);
  int i;
  node->dfs_visited=TRUE;
  for (i=0; i<node->noNeighbor; i++){
    if(node->neighbor[i]->dfs_visited == FALSE){
      // printf("ENTROU!!! node %d  \n",node->key);
      nEdges[compIndex]+=1; //add edge
      add_child(node, node->neighbor[i]);
      DFS_visit(node->neighbor[i],compIndex);
    }
  }
}

void DFS(Node* node[],int size)
{
  int i;
  for (i =0; i< size; i++){
    if (node[i]->dfs_visited == FALSE){
        DFS_visit(node[i],i);
    }
  }
  // return newTree;
}

Node* insert_node (Node* parent,int conf[] )
{
  Node* aux,*a,*new;
  int tmp,i,key,pos;
  
  // Head was created previously so it
  // just needs to be inserted into the graph
  if (hasCreatedHead == -1)
    {
      new = create_head(conf);
      graph[cCompCreate][0] = new;      
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
      if (graph[cCompCreate][tmp]->key > key)
	{
	  // This should iterate 1 maybe 2 times
	  while (graph[cCompCreate][tmp]->key > key)
	    {
	      tmp--;
	      if (tmp < 0)
		  break;
		
	    }	 
	}
      else if (graph[cCompCreate][tmp]->key < key)
	{
	  // 1 or 2 times too.
	  while (graph[cCompCreate][tmp]->key < key)
	    {
	      if (graph[cCompCreate][tmp+1] == NULL)
		  break; 
	      // if the next key is already greater than ok, it doesnt need to search anymore
	      if (graph[cCompCreate][tmp + 1]->key > key || graph[cCompCreate][tmp+1] == NULL)
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
	  aux = graph[cCompCreate][tmp];
	  pos = tmp - 1;
	  graph[cCompCreate][tmp] = graph[cCompCreate][tmp - 1];
	  tmp++;
	  i = tmp;
	  // push to the right every node with greater key once
	  while (i < it)
	    {
	      a = graph[cCompCreate][i];
	      graph[cCompCreate][i] = aux;
	      aux = a;
	      i++;
	    }
	  // For the last item is just need  put the aux into it
	  graph[cCompCreate][i] = aux;
	}
      else if (tmp + 1 == it) // There is exactly one node to reach graph[cCompCreate][it]
	{
	  tmp++;
	  aux = graph[cCompCreate][tmp];
	  pos = tmp;
	  graph[cCompCreate][it] = aux;
	}
      else// new node must be set to the last position
	{
	  pos = it;
	}
      printf(">pos = %d<\n",pos );
      new = create_neighbor(parent, conf);
      graph[cCompCreate][pos] = new;

      return new;  
    }
    
  return NULL;
}

int binary_search_mod (int begin, int end, int key)
{
  int pivot;
  while (begin <= end)
    {
      pivot = (begin+end)/2;  
      if (key == graph[cCompCreate][pivot]->key)
	{
	  foundKey = pivot;
	  break;
	}
      else if (key < graph[cCompCreate][pivot]->key)
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

void print_indent(int indent){
  int i;
  for(i=0; i<indent; i++)
    printf(" "); 
}

void print_DFS(Node* node, int indent){
  int i;
  // print_indent(indent);
  printf("%d\n",node->key);
  if(node->noChild > 0){
    indent++;
    for(i=0; i<MAX_CHILDREN; i++){
      if(node->child[i] != NULL){
        print_DFS(node->child[i],indent);
      } 
    }
    indent--;
  }
}

int main (void)
{

  printf("Edges %d\n", GraphEdges);
   
  int cOne[9] = {0,1,2,3,4,5,6,7,8};
  int cTwo[9] = {0,2,1,3,4,5,6,7,8};

  // Create the connected component one
  init_create_graph(1);  
  create_graph(cOne);

  cCompSize = it;

  printf ("Finished first CC, size:%d \n", cCompSize);
   // Create the connected component two
  init_create_graph(2);  
  create_graph(cTwo);

  printf("Heads:\n");
  print_node(head[0]);
  print_node(head[1]);

  DFS(head,NO_CCOMP);
  printf("IMPRIMINDO 1 - %d edges\n",nEdges[0]);
  print_DFS(head[0],0);
  printf("IMPRIMINDO 2 - %d edges\n",nEdges[1]);
  print_DFS(head[1],0);
  printf("Edges %d\n", GraphEdges);
  
  printf ("Size second component: %d\n", it+1);
  free_list(graph[0], cCompSize);
  printf ("Freed first cc\n");
  fflush(stdout);
  free_list(graph[1], cCompSize);
  printf ("Freed second cc\n");

  

  return 0;
}
