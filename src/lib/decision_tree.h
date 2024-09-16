#ifndef DECISION_TREE_H
#define DECISION_TREE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct DecisionNode {
    char *condition;               
    int decision;                  
    struct DecisionNode *yes;      
    struct DecisionNode *no;       
} DecisionNode;

// Função para criar um novo nó de decisão
DecisionNode* create_node(char *condition, int decision);

// Função para liberar a memória da árvore
void free_tree(DecisionNode *root);

// Funcao para criar a arvore de decisao
DecisionNode* build_decision_tree(); 

// Funcao para retornar a decisao
int get_decision(DecisionNode *root, const char *log_message);

#endif