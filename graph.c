#include<stdlib.h>
#include<stdio.h>
#include<limits.h>
#include "fifo.h"
#define NO_CCOMP 2

static Node* graph[NO_CCOMP][MAX_GRAPH_SIZE];
static Node* plays[MAX_GRAPH_SIZE];
static int noPlays = 0;
static int MAX_DIST = 0;
static int cCompCreate;
unsigned int it;
static Node* head[NO_CCOMP];
static int cCompSize;
static int dfsEdges[NO_CCOMP];
static int GraphEdges = 0;
static int bfsEdges[NO_CCOMP];
static int preTime;
// Closest key index found from the input key 
static int closestKey;
static int foundKey;
static int hasCreatedHead;
static int endPoint[9] = {1,2,3,4,5,6,7,8,0};
static int endKey;

void create_graph(int config[]);
void init_search();
Node* insert_node (Node* parent,int conf[]);
int binary_search_mod (int begin, int end, int key);
void create_iterative(Node* parent);
void BFS(Node* node);
void init_create_graph(int cComp);
void set_max_dist(Node* node, int dist);
void find_max_dist(Node* node,int dist);
void DFS(Node* node[], int nComp);
void DFS_visit(Node* node, int compIndex);

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

	      new = insert_node(father, nConfig);
        

	      if (new != NULL)
		{// Stack the new node up
		  queue_put(new);
		}
	      else if (foundKey > -1) // There is a found key, just need set in the neighbors
		{
		  if  (father->noNeighbor > 3)
		    {
		      printf("Tryed to insert more than 4 nodes!\n");
		      fflush(stdout);
		      exit(0);
		    }
		  if (graph[cCompCreate][foundKey] != NULL)
		    {
		      father->neighbor[father->noNeighbor] = graph[cCompCreate][foundKey];
		      father->noNeighbor++;
		    }
		  else
		    {
		      printf ("Watch out! there is no such neighbor to be included!(create iteratives)\n");
		      fflush(stdout);
		      exit(0);
		    }
		}
	    }
	}
    }
  GraphEdges += edges/2;
}

void create_graph(int config[]){

  Node* new = NULL;

  head[cCompCreate] = insert_node(new,config);

  dfsEdges[cCompCreate] = 0;
  bfsEdges[cCompCreate] = 0;
  
  create_iterative(head[cCompCreate]);
}


void DFS_visit(Node* node, int compIndex)
{
  int i;
  //Mark the node as visited so it won't become child to other nodes
  node->dfs_visited=TRUE;
  node->time = preTime;
  preTime++;
  print_node(node);
  for (i=0; i<node->noNeighbor; i++){
    if(node->neighbor[i]->dfs_visited == FALSE){
      //Add neighbor as a child of node and add an edge to graph
      bfsEdges[compIndex]+=1;
      add_child(node, node->neighbor[i]);
      //Use recently added child as next node to be explored
      DFS_visit(node->neighbor[i],compIndex);
    }
  }
}

void DFS(Node* node[],int size)
{
  int i;
  preTime = 1;
  //Runs DFS from each node in node[]
  for (i =0; i< size; i++)
    {
      if (node[i]->dfs_visited == FALSE)
	{
	  DFS_visit(node[i],i);
	}
    }
}

void set_max_dist(Node* node, int dist)
{
  int i;
  printf("dist: %d\n", dist);
  if (dist > MAX_DIST)
    {
      for (i = 0; i < noPlays; i++)
	{
	  plays[i] = NULL;
	}
      plays[0] = node;
      noPlays = 1;
      MAX_DIST = dist;
    }
  else if (dist == MAX_DIST)
    {
      plays[noPlays] = node;
      noPlays++;
    }
}

void BFS(Node* node)
{
  int j;
  queue_init();
  Node* top;
  queue_put(node);
  node->bfs_visited = TRUE;
  while(queue_length() > 0){
    top = queue_get();
    for(j=0; j<top->noNeighbor; j++){
      if(top->neighbor[j] == NULL)
	{
	  printf("Tried to access a NULL neighbor\n");
	  fflush(stdout);
	}
      else if(top->neighbor[j]->bfs_visited == FALSE){
        bfsEdges[0]++;
        top->neighbor[j]->bfs_visited = TRUE;
        add_child(top,top->neighbor[j]);
        queue_put(top->neighbor[j]);
      }
    }
  }
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

//Prints the amount of indents before printing node
void print_indent(int indent)
{
  int i;
  for(i=0; i<indent; i++)
    printf(" "); 
}

void print_DFS(Node* node, int indent){
  int i;
  print_indent(indent);
  printf("%d\n",node->key);
  if(node->noChild > 0)
    {
      //If there is child, add indentation for the child node
      indent++;
      for(i=0; i<MAX_CHILDREN; i++)
	{
	  if(node->child[i] != NULL)
	    {
	      print_DFS(node->child[i],indent);
	    } 
	}
      //All children added, remove one indent
      indent--;
    }
}

void find_max_dist(Node* node,int dist)
{
  int i;
  queue_init();
  if(node->noChild > 0)
    {
      dist++;
      for(i=0; i<MAX_CHILDREN; i++)
	{
	  if(node->child[i] != NULL)
	    {
	      if(dist >= MAX_DIST)
		set_max_dist(node, dist);
	      find_max_dist(node->child[i], dist);
	    } 
	}
      dist--;
    }
}
void reset_childs(int cCIndex)
{
  int i, j = 0;
  while (j < cCompSize)
    {
      for (i = 0; i < graph[cCIndex][j]->noChild; i++)
	{
	  graph[cCIndex][j]->child[i] = NULL;
	}
      graph[cCIndex][j]->noChild = 0;
      j++;
    }
}

int main (void)
{
  int cOne[9] = {0,1,2,3,4,5,6,7,8};
  int cTwo[9] = {0,2,1,3,4,5,6,7,8};
  
  // Create the connected component one
  endKey = generate_key(endPoint);

  init_create_graph(1);  
  create_graph(cOne);

  printf ("Finished first CC\n");

  // Create the connected component two
  init_create_graph(2);  
  create_graph(cTwo);

  printf ("Finished second CC\n");
  
  // Since first component has the same size as the second
  cCompSize = it;
  printf("Size of comonents %d", cCompSize + 1);
  
  printf("Endpoint Key: %d\n", generate_key(endPoint));
    
  // Set the connected component to be the first and the search
  cCompCreate = 0;
  init_search();
  binary_search_mod (0,cCompSize,endKey);
  // Indeed it is on the first connected component and it is on the foundKey index 
  if (foundKey >  1)
    printf ("Found endPoint first CC!!!\n");

  //Search for longest distance from endPoint node
  BFS(graph[0][foundKey]);
  printf("\nStarting search for maximum distance from endPoint\n");
  find_max_dist(graph[0][foundKey],0);
  printf("----MAX DIST FOUND: %d----\n There are two configurations found in the longest layer!\n", MAX_DIST);

  reset_childs(0);
  reset_childs(1);

  DFS(head, NO_CCOMP);
  //print_DFS(head[0], 0);
  
  free_list(graph[0], cCompSize);
  printf ("Freed first cc\n");
  fflush(stdout);
  free_list(graph[1], cCompSize);
  printf ("Freed second cc\n");

  return 0;
}
