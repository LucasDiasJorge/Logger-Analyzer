#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct DecisionNode {
    char *condition;               
    char *decision;                  
    struct DecisionNode *yes;      
    struct DecisionNode *no;       
} DecisionNode;

// Função para criar um novo nó de decisão
DecisionNode* create_node(char *condition, char *decision) {
    DecisionNode *node = (DecisionNode *)malloc(sizeof(DecisionNode));
    if (!node) {
        perror("Failed to allocate memory for DecisionNode");
        exit(EXIT_FAILURE);
    }
    node->condition = strdup(condition);
    node->decision = strdup(decision);
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
    free(root->decision);
    free(root);
}

// Define your decision tree nodes
DecisionNode* build_decision_tree() {
    // Create nodes
    DecisionNode *root = create_node("alerta de seguranca", "ALERT");
    root->yes = create_node("Falha de segmentacao", "RESTART");
    root->no = create_node("erro de memoria", "SHUTDOWN");

    return root;
}

// Função para buscar a decisão na árvore de decisão
const char* get_decision(DecisionNode *root, const char *log_message) {
    if (root == NULL) {
        return NULL;
    }

    if (strstr(log_message, root->condition) != NULL) {
        return root->decision;
    } else {
        // Check both branches
        const char* decision = get_decision(root->yes, log_message);
        if (decision == NULL) {
            decision = get_decision(root->no, log_message);
        }
        return decision;
    }
}
