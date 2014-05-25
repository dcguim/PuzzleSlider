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
  //print_node(parent);
}

void create_graph(int config[]){
  
  Node* new = NULL;

  head[cCompCreate] = insert_node(new,config);

  create_iterative(head[cCompCreate]);
void DFS_visit(Tree* tree, Node* node)
{
  // printf("node visited - %d with %d neighbors\n", node->dfs_visited,node->noNeighbor);
  int i;
  node->dfs_visited=TRUE;
  for (i=0; i<node->noNeighbor; i++){
    if(node->neighbor[i]->dfs_visited == FALSE){
      // printf("ENTROU!!! node %d  \n",node->key);
      add_child(tree, node, node->neighbor[i]);
      DFS_visit(tree, node->neighbor[i]);
    }
}

Tree* DFS(Node* node)
{
  Tree* newTree = create_tree(node);
  // node->dfs_visited = TRUE;
  // for (i =0; i< node->noNeighbor; i++){
    // if(node->neighbor[i]->dfs_visited == FALSE){
      DFS_visit(newTree, newTree->root);
    // }
  // }
  return newTree;
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

int main (void)
{
  int cOne[9] = {0,1,2,3,4,5,6,7,8};
  int cTwo[9] = {0,2,1,3,4,5,6,7,8};

  // Create the connected component one
  init_create_graph(1);  
  // int cThree[9] = {2,1,5,3,4,0,6,7,8};
  // int cFour[9] = {1,0,2,3,4,5,6,7,8};

  // Node* n1 = create_head(cOne);
  // Node* n2 = create_head(cTwo);
  // Node* n3 = create_head(cThree);
  // Node* n4 = create_head(cFour);
  // n1->key=1;
  // n2->key=2;
  // n3->key=3;
  // n4->key=4;

  // nodes[0]=n1;
  // nodes[1]=n2;
  // nodes[2]=n3;
  // nodes[3]=n4;
  
  // n1->neighbor[0]=n2;
  // n1->noNeighbor++;
  // n1->neighbor[1]=n4;
  // n1->noNeighbor++;
  // n4->neighbor[0]=n3;
  // n4->noNeighbor++;

  // Tree* test = DFS_test(nodes,4);

  // for(i=0;i<4;i++)
  //   print_node(nodes[i]);

  // print_tree(test->root,0);



  // Tree* t1 = create_tree(n1);

  // add_child(t1,n1,n2);
  // add_child(t1,n1,n4);
  // add_child(t1,n4,n3);

  // printf("Nodes: %d Edges: %d\n",t1->nNodes,t1->nEdges);
  // print_tree(t1->root,0);
   
  Tree* tree;

  create_graph(cOne);
  
  tree = DFS(head);

  printf("\n\nPRINTING TREE - %d NODES %d EDGES\n\n",tree->nNodes,tree->nEdges);
  print_tree(tree->root,0);

  // for (int i = 0; i < it; ++i)
  // {
  //   print_node(graph[i]);
  // }

  cCompSize = it;

  printf ("Finished first CC, size:%d \n", cCompSize);
   // Create the connected component two
  init_create_graph(2);  
  create_graph(cTwo);

printf ("Size second component: %d\n", it+1);
  free_list(graph[0], cCompSize);
  printf ("Freed first cc\n");
  fflush(stdout);
  free_list(graph[1], cCompSize);
  printf ("Freed second cc\n");

  return 0;
}
