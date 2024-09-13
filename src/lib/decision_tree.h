#ifndef DECISION_TREE_H
#define DECISION_TREE_H

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
DecisionNode* create_node(char *condition, char *action);

// Função para liberar a memória da árvore
void free_tree(DecisionNode *root);

#endif