#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct DecisionNode {
    char *condition;               
    char *action;                  
    struct DecisionNode *yes;      
    struct DecisionNode *no;       
} DecisionNode;

// Função para criar um novo nó de decisão
DecisionNode* create_node(char *condition, char *action) {
    DecisionNode *node = (DecisionNode *)malloc(sizeof(DecisionNode));
    node->condition = strdup(condition); 
    node->action = strdup(action);        
    node->yes = NULL;                    
    node->no = NULL;
    return node;
}

// Função para liberar a memória da árvore
void free_tree(DecisionNode *root) {
    if (root == NULL) return;
    free_tree(root->yes);
    free_tree(root->no);
    free(root->condition);
    free(root->action);
    free(root);
}
