#ifndef DECISION_NODE_H
#define DECISION_NODE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definição das ações
enum Action {
    ACTION_ALERT,
    ACTION_RESTART,
    ACTION_SHUTDOWN,
    ACTION_LOG,
    ACTION_REPAIR,
    ACTION_SLOWDOWN
};

// Estrutura do nó da árvore
typedef struct DecisionNode {
    int action;
    struct DecisionNode *left;
    struct DecisionNode *right;
} DecisionNode;

DecisionNode *root;

// Funções da árvore de decisão
DecisionNode* create_node(int action);
void free_tree(DecisionNode *root);
const DecisionNode* build_decision_tree();
int get_action(DecisionNode *root, int error_severity);
int make_decision(const char* log_message, DecisionNode* decision_tree);

#endif
