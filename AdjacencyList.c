#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#define CONFIG_SIZE 9
#define TRUE 1
#define FALSE 0
/*
TODO
testar which neightbor serao explorados para as 9 possíveis posições
*/

typedef int bool;

/**** Estrura de um elemento da lista ****/
typedef struct node
{
  int config[9];
  struct node* neightbor;
  int missPiecePos;
  bool visited; 
}Node;
/*****************************************/

/**** Protótipos  ****/
void create_list( int startPoint[] );
void create_neightbor ( Node* curr, const int config[] );
void which_neightbors (Node* curr, int neightbors[]);
void free_list(Node* begin);
/********************/


/**** Varáveis  ****/
static Node * head;
/*******************/

int main( void )
{
  int i;
  int startPoint[9] = {1,2,3,4,5,6,4,8,0};
  int config[9] = {1,2,3,4,5,0,7,8,6};
  //char noExp[4];
  
  create_list( startPoint );
  
  for (i = 0; i < CONFIG_SIZE; i++)
    {
      printf ("begin: %d\n", head->config[i]);
    }
  printf ("missCurr: %d", head->missPiecePos);
  fflush(stdout);
  //which_neightbors (head, noExp);
  /*
  printf ("no cima %d", noExp[0]);
  printf ("no esq %d", noExp[1]);
  printf ("no baixo %d", noExp[2]);
  printf ("no dir %d", noExp[3]);
  */
  create_neightbor (head, config);

  printf ("print neightbor\n");
  
  for (i = 0; i < CONFIG_SIZE; i++)
    {
      printf ("nB: %d\n", head->neightbor->config[i]);
    }
  printf ("missCurr: %d", head->neightbor->missPiecePos);
  
  free_list(head);
  return 0;
}

void create_list( int startPoint[] )
{
  int i;

  head = (Node*) malloc(sizeof(Node));
  if (head == NULL)
    {
      exit (0);
    }
  for (i = 0; i < CONFIG_SIZE; i++)
    {
      head->config[i] = startPoint[i];
      if (startPoint[i] == 0)
	head->missPiecePos = i;
    }

  head->neightbor = NULL;
}

void which_neightbors (Node* curr, int neightbors[4])
{
  int i;
  // inicializa o vetor de possíveis vizinhos com -1, i.e. não há vizinhos;
  for (i = 0; i < 4; i++)
    neightbors[i] = -1;

  // Existe um vizinho à esquerda
  if ((curr->missPiecePos % 3) == 2)
    neightbors[1] = curr->missPiecePos -1;
  // Existe um vizinho à direita & esquerda
  else if ((curr->missPiecePos % 3) == 1)
    {
     neightbors[3] = curr->missPiecePos + 1;
     neightbors[1] = curr->missPiecePos - 1;
    }
  // Existe um vizinho à esquerda
  else //((curr->missPiecePos % 3) == 0)
    neightbors[3] = curr->missPiecePos + 1;

  // Existe um vizinho em baixo
  if ((curr->missPiecePos / 3) == 0)
    neightbors[2] = curr->missPiecePos + 3;
  // Existe um vizinho em cima & baixo
  else if ((curr->missPiecePos / 3) == 1)
    {
     neightbors[0] = curr->missPiecePos - 3;
     neightbors[2] = curr->missPiecePos + 3;
    }
  // Existe um vizinho em cima
  else //((curr->missPiecePos / 3) == 2)
    neightbors[0] = curr->missPiecePos - 3;
}


void create_neightbor ( Node* curr, const int config[] )
{
  int i;
  Node* new;
   
  new = (Node*) malloc(sizeof(Node));
  if (new == NULL)
    {
      exit (0);
    }

  curr->neightbor = new;
  new->neightbor = NULL;
  for (i = 0; i < CONFIG_SIZE; i++)
    {
      new->config[i] = config[i];
      // Indica qual a posição no array está o buraco
      if (config[i] == 0)
	new->missPiecePos = i;
    }
}

void free_list (Node* begin)
{
  Node* aft;
  Node* ant;
  aft = begin;
  while (aft != NULL)
    {
      ant = aft;
      aft = aft->neightbor;
      free(ant);
    }
  printf ("lista liberada!\n");
}
