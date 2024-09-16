#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lib/decision.h"

// Função para criar um novo nó de decisão
DecisionNode* create_node(char *condition, int decision) {
    DecisionNode *node = (DecisionNode *)malloc(sizeof(DecisionNode));
    if (!node) {
        perror("Failed to allocate memory for DecisionNode");
        exit(EXIT_FAILURE);
    }
    node->condition = strdup(condition);
    if (!node->condition) {
        perror("Failed to duplicate condition string");
        free(node); // Ensure no memory leak
        exit(EXIT_FAILURE);
    }
    node->decision = decision;
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

DecisionNode* build_decision_tree() {
    // Criar nós
    DecisionNode *root = create_node("system error", ACTION_ALERT);
    root->yes = create_node("disk full", ACTION_RESTART);
    root->no = create_node("memory leak", ACTION_SHUTDOWN);
    
    root->yes->yes = create_node("disk error", ACTION_REPAIR);  // Missing in your logic
    root->yes->no = create_node("disk not found", ACTION_LOG);
    
    root->no->yes = create_node("high CPU usage", ACTION_SLOWDOWN);  // Missing in your logic
    root->no->no = create_node("memory access violation", ACTION_RESTART);
    
    root->no->yes->yes = create_node("disk usage above threshold", ACTION_ALERT);
    root->no->yes->no = create_node("CPU usage normal", ACTION_LOG);

    root->no->no->yes = create_node("memory corruption", ACTION_REPAIR);  // Missing in your logic
    root->no->no->no = create_node("invalid memory address", ACTION_SHUTDOWN);

    return root;
}

// Função para buscar a decisão na árvore de decisão
int get_decision(DecisionNode *root, const char *log_message) {
    if (root == NULL) {
        return -1; // Indica que não foi encontrado
    }

    if (strstr(log_message, root->condition) != NULL) {
        return root->decision; // Retorna a decisão como um código int
    } else {
        // Check both branches
        int decision = get_decision(root->yes, log_message);
        if (decision == -1) {
            decision = get_decision(root->no, log_message);
        }
        return decision;
    }
}