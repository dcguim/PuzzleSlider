#include "tree.h"

Tree* create_tree(Node* root){
  Tree* new = (Tree*)malloc(sizeof(Tree));
  new->root = root;
  new->nNodes = 1;
  new->nEdges = 0;
  return new;
}

// void add_child(Tree* tree, Node* parent, Node* child){
//   int i;
//   for (i=0; parent->child[i]; i++){ //Get the correct position for the child
//   }
//   parent->child[i] = child;
//   child->parent = parent;
//   parent->noChild++;
//   tree->nNodes+=1;
//   tree->nEdges+=1;
// }

// void print_indent(int indent){
//   int i;
//   for(i=0; i<indent; i++)
//     printf("  "); 
// }

// void print_tree(Node* node, int indent){
//   int i;
//   print_indent(indent);
//   printf("%d\n",node->key);
//   if(node->noChild > 0){
//     indent++;
//     for(i=0; i<MAX_CHILDREN; i++){
//       if(node->child[i] != NULL){
//         print_tree(node->child[i],indent);
//       } 
//     }
//     indent--;
//   }
// }

