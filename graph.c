#include"fifo.h"
#include<stdlib.h>
#include<stdio.h>
#include<limits.h>

static Node* graph[MAX_GRAPH_SIZE];
static unsigned int it;

// Closest key index found from the input key 
static int closestKey;
static int foundKey;

void init_search()
{
  closestKey = -1;
  foundKey = -1;
}

int binary_search (int begin, int end, int key);

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

void insert_node (Node* parent,int conf[] )
{
  Node* aux,*a,*new;
  int tmp,i,key,pos;
  //Node* new;
  key = generate_key(conf);
  
  binary_search (0,it,key);
  // node was not found
  if (foundKey == -1 && closestKey != -1)
    {
       printf("\n Not found! closest found:[%d]\n", closestKey);
      tmp = closestKey;
      if (graph[tmp]->key > key)
	{
	  // This should iterate 1 maybe 2 times
	  while (graph[tmp]->key > key)
	    {
	      tmp--;
	    }
	 
	}
      else if (graph[tmp]->key < key)
	{
	  // 1 or 2 times too.
	  while (graph[tmp]->key < key)
	    {
	      tmp++;
	    }
	}
      printf("tmp:%d\n",tmp);
      // I want to leave a hole for the node to be inserted, this explain the jump
      tmp = tmp + 2;
      // The size of graph must be increased by one
      it ++;
      // Open the hole(at tmp - 1) and push forward once
      aux = graph[tmp];
      pos = tmp - 1;
      graph[tmp] = graph[tmp - 1];
      tmp++;
      i = tmp;
      while (i < it)
	{
	  a = graph[i];
	  graph[i] = aux;
	  aux = a;
	  i++;
	}
      // For the last item is just need  put the aux into it
      graph[i] = aux;
      new = create_neighbor(parent, conf);
      printf("Pos %d the new node will be inserted!\n",tmp);
      graph[pos] = new;
      
      for (i = 0; i <= it; i++)
	printf("[%d]:%d\n",i, graph[i]->key);
      
    
    }
  else if (foundKey >= 0)
    printf("\nFound! [%d]\n", foundKey);
    
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
  int i, key;
  
  // some nodes
  Node* one,*two,*three,*four,*five;
  // their respective configs
  int cFive[9] = {0,1,2,3,4,5,6,8,8};
  int cFour[9] = {1,0,2,3,4,5,6,7,8};
  int cTwo[9] = {1,2,3,4,5,6,7,8,0};
  int cThree[9] = {1,2,3,4,5,6,7,0,8};
  int cOne[9] = {2,1,3,4,5,6,7,8,0};
  
  int cIns[9] = {0,1,2,3,4,5,6,7,8};
  
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

  printf ("it = %d\n", it);
  free_list(graph, it);

  return 0;
}
