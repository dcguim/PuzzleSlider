#include"fifo.h"
#include<stdlib.h>
#include<stdio.h>
#include<limits.h>

static Node* graph[MAX_GRAPH_SIZE];
unsigned int it;
static Node* head;

// Closest key index found from the input key 
static int closestKey;
static int foundKey;
static int hasCreatedHead;

int binary_search (int begin, int end, int key);
void create_graph(int config[]);
void init_search();
Node* insert_node (Node* parent,int conf[]);
int binary_search (int begin, int end, int key);
void create_recursive(Node* parent);


void init_search()
{
  closestKey = -1;
  foundKey = -1;
}

void create_recursive(Node* parent)
{
  Node* new;
  int i,j;
  int nBors[MAX_NEIGHBORS];
  int nConfig[CONFIG_SIZE];

  printf("wooh\n");
  fflush(stdout);
  which_neighbors(parent,nBors);
  print_node(parent);
  printf("wooh\n");
  fflush(stdout);
  
   // Create the necessary nodes  
  for (i = 0; i< MAX_NEIGHBORS; i++)
    {
      // if neighbor should be created
      if (nBors[i] != -1)
	{
	  // Copy the previous configuration
	  for (j = 0; j< CONFIG_SIZE; j++) 
	    nConfig[j] = parent->config[j];

	  // Swap the hole with the nBors[i]
	  nConfig[parent->missPiecePos] = nConfig[nBors[i]];
	  nConfig[nBors[i]] = 0;

	  printf("woohInsertBefore\n");
	  fflush(stdout);
	  new = insert_node(parent, nConfig);
	  printf("woohInsertAfter\n");
	  fflush(stdout);
	  
	  if (new != NULL)
	    create_recursive(new);

	   printf("woohNULLConditionAfter\n");
	  fflush(stdout);
	}
    }
  print_node(parent);
}

void create_graph(int config[]){
  
  Node* new = NULL;

  head = insert_node(new,config);

  head->visited = TRUE;
  //print_node(graph[0]);

  create_recursive(head);
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

  /*for (i = 0; i < CONFIG_SIZE; i++)
    {
      printf ("[%d]:%d", i, conf[i]);
      if (((i+1)%3) == 0)
	printf("\n");
    }*/
  
  // Head is created previously so it just needs to be inserted into the graph
  if (hasCreatedHead == -1)
    {
      new = create_head(conf);
      graph[0] = new;
      //print_node(graph[it]);
      hasCreatedHead = 1;
      return new;
    }

   printf("woohSearchBefore\n");
	  fflush(stdout);
  key = generate_key(conf);
  init_search();
  binary_search (0,it,key);
 printf("woohSearchAfter\n");
	  fflush(stdout);
 
  printf ("foundkey :%d / closestkey: %d\n", foundKey, closestKey);
  fflush(stdout);
  // node was not found
  if (foundKey == -1 && closestKey != -1)
    {
      printf("\n Not found! closest found:[%d]\n", closestKey);
      fflush(stdout);
      tmp = closestKey;
      if (graph[tmp]->key > key)
	{
	  printf("woohGraph[key]>key\n");
	  fflush(stdout);
	  // This should iterate 1 maybe 2 times
	  while (graph[tmp]->key > key)
	    {
	      tmp--;
	      if (tmp < 0)
		{
		  printf("Node %d must be set to the first position\n", generate_key(conf));
		  fflush(stdout);
		  break;
		}
	    }	 
	}
      else if (graph[tmp]->key < key)
	{
	   printf("woohGraph[key]<key\n");
	  fflush(stdout);
	  // 1 or 2 times too.
	  while (graph[tmp]->key < key)
	    {
	      if (graph[tmp+1] == NULL)
		{
		  printf("node %d already is in the last position of the array!\n", generate_key(conf));
		  fflush(stdout);
		  break;
		}
	      
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

int binary_search (int begin, int end, int key)
{
  int pivot;
  
  // Stop condition
  if (begin >= end)
    {
      if (begin == end)
	{
	  if (graph[begin]->key == key)
	    {
	      foundKey = begin;
	    }
	  else
	    closestKey = begin;
	  return -1;
	}
      
      printf("begin:%d end:%d\n", begin, end);
      if (end < 0)
	closestKey = 0;
      else if ( begin > it)
	closestKey = it;
      else
	closestKey = end; 
	 
      return -1;
    }
  // The array was previously sorted so will be a good heuristic to 
  // set the pivot to the middle of the array
  pivot = begin + (end - begin)/2;
  printf ("\npivot:%d\n", pivot);
  if (key == graph[pivot]->key)
    {
      foundKey = pivot;
    }
  else if (key < graph[pivot]->key)
    {
      binary_search(begin, pivot - 1, key);
    }
  else
    {
      binary_search(pivot + 1, end, key);
    }
  return 0;
 }

int main (void)
{
  int i;// key;
  // some nodes
  /*
  // their respective configs
  int cFive[9] = {0,1,2,3,4,5,6,8,8};
  int cFour[9] = {1,0,2,3,4,5,6,7,8};
  int cTwo[9] = {2,2,2,2,2,2,7,8,0};
  int cThree[9] = {1,2,3,4,5,6,7,0,8};*/
  int cOne[9] = {0,1,2,3,4,5,6,7,8};
  
  //int cIns[9] = {0,1,2,3,4,5,6,8,9};
  hasCreatedHead = -1;
  create_graph(cOne);
  
  /*
  one = create_head(cOne);
  it = 0;
  graph[it] = one;
  
  two = create_neighbor(one, cTwo);
  it++;
  graph[it] = two;
  
  three = create_neighbor(one, cThree);
  it++;
  graph[it] = three;
  
  four = create_neighbor(one, cFour);
  it++;
  graph[it] = four;
  
  five = create_neighbor(two, cFive);
  it++;
  graph[it] = five;
  

  for (i = 0; i < 9; i++)
    {
      one->config[i] = cOne[i];
      two->config[i] = cTwo[i];
      three->config[i] = cThree[i];
      four->config[i] = cFour[i];
      five->config[i] = cFive[i];
    }
  one->key = generate_key(cOne);
  two->key = generate_key(cTwo);
  three->key = generate_key(cThree);
  four->key = generate_key(cFour);
  five->key = generate_key(cFive);
  
  key = generate_key(cIns);

  printf("\nKeys generated: [0]:%d,\n[1]:%d,\n[2]:%d,\n[3]:%d,\n[4]:%d",
  	 one->key,two->key,three->key,four->key,five->key);

  printf("\ntobe inserted %d", key);
  init_search();
  insert_node(five, cIns);
  */
  //print_node(graph[0]);
  printf ("it = %d\n", it);
  for (i = 0; i <= it; i++)
    print_node(graph[i]);
  
  free_list(graph, it);

  return 0;
}
