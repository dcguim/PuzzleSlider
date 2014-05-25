#include "tree.h"

Tree* create_tree(Node* root){
  Tree* new = (Tree*)malloc(sizeof(Tree));
  new->root = root;
  new->nNodes = 1;
  new->nEdges = 0;
  return new;
}

void add_child(Tree* tree, Node* parent, Node* child){
  int i;
  for (i=0; parent->child[i]; i++){ //Get the correct position for the child
  }
  parent->child[i] = child;
  child->parent = parent;
  parent->noChild++;
  tree->nNodes+=1;
  tree->nEdges+=1;
}

void print_indent(int indent){
  int i;
  for(i=0; i<indent; i++)
    printf("  "); 
}

void print_tree(Node* node, int indent){
  int i;
  print_indent(indent);
  printf("%d\n",node->key);
  if(node->noChild > 0){
    indent++;
    for(i=0; i<MAX_CHILDREN; i++){
      if(node->child[i] != NULL){
        print_tree(node->child[i],indent);
      } 
    }
    indent--;
  }
}

// void printAST(Node* root,int tabs)
// { 
//   Node* type=tree->root;
//   printIndent(tabs);
//   printf("%d",node->key);
//   if(node->sVal == NULL)
//     if(node->type==AST_LITNUM)
//       if(node->symbolType!=NULL)
//         printf(" val:%d - %s (line %d)",node->iVal,getSymbolType(node),node->line);
//       else
//         printf(" val:%d (line %d)",node->iVal,node->line);
//     else    
//       printf( "(line %d)", node->line);
//   else if(node->symbolType!=NULL)
//     printf(" %s - %s (line %d)",node->sVal,getSymbolType(node),node->line);
//   else
//     printf(" %s (line %d)",node->sVal,node->line);
//   printf( "\n");

//   if(node->firstChild != NULL){ 
//     printAST(node->firstChild,tabs+1) ;
//   }
//   if(node->prox != NULL)
//     printAST(node->prox,tabs) ;
// }

