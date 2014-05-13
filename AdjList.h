#define TRUE 1
#define FALSE 0
#define CONFIG_SIZE 9
#define MAX_NEIGHBORS 4
typedef int bool;

/**** Node of a List ****/
typedef struct node
{
  int config[9];
  struct node* neighbor;
  int missPiecePos;
  bool visited; 
}Node;
/*****************************************/

/****************** Prototypes  **************************/
Node * create_list(int startPoint[] );

void create_neighbor ( Node* curr, const int config[] );

/** up: neighbors[0] / left: neighbors[1]
    down: neighbors[2] / right: neighbors[3]  **/
void which_neighbors (Node* curr, int neighbors[]);

void free_list(Node* begin);

void print_node (Node* n);
/*********************************************************/
