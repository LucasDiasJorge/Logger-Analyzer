#include "lib/decision_node.h"
#include "lib/error_map.h"

// Cria um nó da árvore de decisão
DecisionNode* create_node(int action) {
    DecisionNode *node = (DecisionNode *)malloc(sizeof(DecisionNode));
    node->action = action;
    node->left = NULL;
    node->right = NULL;
    return node;
}

DecisionNode *root;

// Libera a árvore da memória
void free_tree(DecisionNode *root) {
    if (root == NULL) return;
    free_tree(root->left);
    free_tree(root->right);
    free(root);
}

// Constrói a árvore de ações baseada na gravidade
DecisionNode* build_decision_tree() {
    // Raiz - ação mais grave
    DecisionNode *root = create_node(ACTION_SHUTDOWN);

    // Subárvore da esquerda (ações menos graves)
    root->left = create_node(ACTION_RESTART);
    root->left->left = create_node(ACTION_ALERT);
    root->left->right = create_node(ACTION_LOG);

    // Subárvore da direita (ações mais graves)
    root->right = create_node(ACTION_REPAIR);
    root->right->right = create_node(ACTION_SLOWDOWN);

    return root;
}

// Retorna a ação baseada na gravidade
int get_action(DecisionNode *root, int severity) {
    if (severity < 3) return root->left->left->action;  // ACTION_ALERT
    if (severity < 5) return root->left->action;        // ACTION_RESTART
    if (severity < 7) return root->right->action;       // ACTION_REPAIR
    return root->action;                                // ACTION_SHUTDOWN
}

int make_decision(const char* log_message, DecisionNode* decision_tree){
    // Busca a severidade do log
    int severity = get_error_severity(log_message);
    
    if (severity != -1) {
        // Determina a ação com base na severidade
        int action = get_action(decision_tree, severity);

        // Executa a ação
        switch(action) {
            case ACTION_ALERT:
                printf("Executando ação: Enviar alerta.\n");
                break;
            case ACTION_RESTART:
                printf("Executando ação: Reiniciar sistema.\n");
                break;
            case ACTION_SHUTDOWN:
                printf("Executando ação: Desligar sistema.\n");
                break;
            case ACTION_LOG:
                printf("Executando ação: Registrar log.\n");
                break;
            case ACTION_REPAIR:
                printf("Executando ação: Reparar sistema.\n");
                break;
            case ACTION_SLOWDOWN:
                printf("Executando ação: Reduzir carga de CPU.\n");
                break;
            default:
                printf("Ação desconhecida.\n");
        }
    } else {
        printf("Erro não encontrado no mapa.\n");
    }

    return 0;
}
