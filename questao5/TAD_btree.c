#include <stdlib.h>
#include <stdio.h>

#define ORDEM 7

typedef struct Node {
    int chaves[ORDEM]; // chaves são o número de elementos que cabe dentro de um nó
    struct Node *filhos[ORDEM+1]; // ordem 7 = 7 filhos +1 extra
    int numChaves;
    int folha;
} Node;

typedef struct BTree {
    Node *root;
} BTree;

BTree* create_tree(void){

    BTree* tree = malloc(sizeof(BTree));

    if (tree == NULL)
        return NULL;

    tree->root = NULL;

    return tree;


}


static Node* create_node(void);
static void insert_non_full(Node* node, int data);
static void split_node(Node* parent, int index);
static Node* search_node(Node* node, int data);
static int is_full(Node* node);
static int is_empty(Node* node);



Node* create_node() {

    Node* node = malloc(sizeof(Node));

    if (node == NULL)
        return NULL;

    node->numChaves = 0;
    node->folha = 1;

    for (int i = 0; i < ORDEM + 1; i++) {
        node->filhos[i] = NULL;
    }

    return node;
}

void insert(BTree* tree, int data) {

    if (tree == NULL)
        return;

    // Verifica se a árvore está vazia
    if (tree->root == NULL) {
        tree->root = create_node();

        if (tree->root == NULL)
            return;

        tree->root->chaves[0] = data;
        tree->root->numChaves = 1;
        return;
    }

    // Impede chaves duplicadas
    if (search_node(tree->root, data) != NULL)
        return;

    // Insere antes de verificar se precisa dividir
    insert_non_full(tree->root, data);

    Node* root = tree->root;

    // Verifica se a raiz ficou com 7 chaves
    if (root->numChaves == ORDEM) {
        Node* newRoot = create_node();

        if (newRoot == NULL) {
            fprintf(stderr, "Erro de alocacao de memoria.\n");
            exit(EXIT_FAILURE);
        }

        newRoot->folha = 0;
        newRoot->filhos[0] = root;

        split_node(newRoot, 0);

        tree->root = newRoot;
    }
}

static void insert_non_full(Node* node, int data) {

    int i = node->numChaves - 1;

    if (node->folha) {

        while (i >= 0 && node->chaves[i] > data) {
            node->chaves[i + 1] = node->chaves[i];
            i--;
        }

        node->chaves[i + 1] = data;
        node->numChaves++;

        return;
    }

    // Encontra o filho correspondente
    while (i >= 0 && node->chaves[i] > data) {
        i--;
    }

    i++;

    // Primeiro insere no filho
    insert_non_full(node->filhos[i], data);

    // Depois verifica se o filho precisa ser dividido
    if (node->filhos[i]->numChaves == ORDEM) {
        split_node(node, i);
    }
}

static void split_node(Node* parent, int index) {

    Node* left = parent->filhos[index];
    Node* right = create_node();

    if (right == NULL) {
        fprintf(stderr, "Erro de alocacao de memoria.\n");
        exit(EXIT_FAILURE);
    }

    right->folha = left->folha;

    // O novo nó recebe as 3 maiores chaves
    right->numChaves = 3;

    for (int j = 0; j < 3; j++) {
        right->chaves[j] = left->chaves[j + 4];
    }

    // Se não for folha, transfere também os filhos
    if (!left->folha) {
        for (int j = 0; j < 4; j++) {
            right->filhos[j] = left->filhos[j + 4];
            left->filhos[j + 4] = NULL;
        }
    }

    // Guarda a mediana que será promovida
    int mediana = left->chaves[3];

    // O nó original mantém as 3 menores chaves
    left->numChaves = 3;

    // Abre espaço para a mediana no pai
    for (int j = parent->numChaves; j > index; j--) {
        parent->chaves[j] = parent->chaves[j - 1];
    }

    // Abre espaço para o novo filho
    for (int j = parent->numChaves; j > index; j--) {
        parent->filhos[j + 1] = parent->filhos[j];
    }

    // Insere a mediana e o novo filho no pai
    parent->chaves[index] = mediana;
    parent->filhos[index + 1] = right;

    parent->numChaves++;
}

#define MIN_CHAVES 3

// Junta dois filhos e a chave separadora do pai
static void merge_children(Node* parent, int index) {
    Node* left = parent->filhos[index];
    Node* right = parent->filhos[index + 1];

    int n = left->numChaves;

    left->chaves[n] = parent->chaves[index];

    for (int j = 0; j < right->numChaves; j++) {
        left->chaves[n + 1 + j] = right->chaves[j];
    }

    if (!left->folha) {
        for (int j = 0; j <= right->numChaves; j++) {
            left->filhos[n + 1 + j] = right->filhos[j];
        }
    }

    left->numChaves = n + 1 + right->numChaves;

    // Remove a chave do pai
    for (int j = index; j < parent->numChaves - 1; j++) {
        parent->chaves[j] = parent->chaves[j + 1];
    }

    // Remove o ponteiro do filho direito
    for (int j = index + 1; j < parent->numChaves; j++) {
        parent->filhos[j] = parent->filhos[j + 1];
    }

    parent->filhos[parent->numChaves] = NULL;
    parent->numChaves--;

    free(right);
}


// Corrige um filho que ficou com menos de 3 chaves
static void fix_underflow(Node* parent, int index) {
    Node* child = parent->filhos[index];

    // Tenta pegar uma chave do irmão esquerdo
    if (index > 0 &&
        parent->filhos[index - 1]->numChaves > MIN_CHAVES) {

        Node* left = parent->filhos[index - 1];

        for (int j = child->numChaves; j > 0; j--) {
            child->chaves[j] = child->chaves[j - 1];
        }

        child->chaves[0] = parent->chaves[index - 1];

        parent->chaves[index - 1] =
            left->chaves[left->numChaves - 1];

        if (!child->folha) {
            for (int j = child->numChaves + 1; j > 0; j--) {
                child->filhos[j] = child->filhos[j - 1];
            }

            child->filhos[0] = left->filhos[left->numChaves];
            left->filhos[left->numChaves] = NULL;
        }

        child->numChaves++;
        left->numChaves--;
        return;
    }

    // Tenta pegar uma chave do irmão direito
    if (index < parent->numChaves &&
        parent->filhos[index + 1]->numChaves > MIN_CHAVES) {

        Node* right = parent->filhos[index + 1];

        child->chaves[child->numChaves] =
            parent->chaves[index];

        parent->chaves[index] = right->chaves[0];

        for (int j = 0; j < right->numChaves - 1; j++) {
            right->chaves[j] = right->chaves[j + 1];
        }

        if (!child->folha) {
            child->filhos[child->numChaves + 1] =
                right->filhos[0];

            for (int j = 0; j < right->numChaves; j++) {
                right->filhos[j] = right->filhos[j + 1];
            }

            right->filhos[right->numChaves] = NULL;
        }

        child->numChaves++;
        right->numChaves--;
        return;
    }

    // Se não pode emprestar, junta com um irmão
    if (index > 0) {
        merge_children(parent, index - 1);
    } else {
        merge_children(parent, index);
    }
}


// Remove recursivamente uma chave
static int remove_from_node(Node* node, int data) {
    int i = 0;

    // Encontra a posição da chave
    while (i < node->numChaves && node->chaves[i] < data) {
        i++;
    }

    // Encontrou a chave
    if (i < node->numChaves && node->chaves[i] == data) {

        // Caso 1: chave está em uma folha
        if (node->folha) {
            for (int j = i; j < node->numChaves - 1; j++) {
                node->chaves[j] = node->chaves[j + 1];
            }

            node->numChaves--;
            return 1;
        }

        // Caso 2: chave está em um nó interno
        // Procura seu predecessor
        Node* pred = node->filhos[i];

        while (!pred->folha) {
            pred = pred->filhos[pred->numChaves];
        }

        int predecessor = pred->chaves[pred->numChaves - 1];

        // Substitui a chave pelo predecessor
        node->chaves[i] = predecessor;

        // Remove o predecessor da posição original
        remove_from_node(node->filhos[i], predecessor);

        // Corrige o filho se perdeu chaves demais
        if (node->filhos[i]->numChaves < MIN_CHAVES) {
            fix_underflow(node, i);
        }

        return 1;
    }

    // Se chegou a uma folha sem encontrar
    if (node->folha) {
        return 0;
    }

    // Continua a busca no filho correspondente
    int removed = remove_from_node(node->filhos[i], data);

    if (removed && node->filhos[i]->numChaves < MIN_CHAVES) {
        fix_underflow(node, i);
    }

    return removed;
}


// Função pública de remoção
int remove_data(BTree* tree, int data) {
    if (tree == NULL || tree->root == NULL) {
        return 0;
    }

    int removed = remove_from_node(tree->root, data);

    // Se a raiz ficou vazia, reduz a altura da árvore
    if (tree->root->numChaves == 0) {
        Node* old_root = tree->root;

        if (old_root->folha) {
            tree->root = NULL;
        } else {
            tree->root = old_root->filhos[0];
        }

        free(old_root);
    }

    return removed;
}


int recover_data(BTree* tree, int data) {
    if (tree == NULL)
        return 0;

    return search_node(tree->root, data) != NULL;
}

void print_node(Node* node, int nivel) {
    if (node == NULL)
        return;

    // Indentação para representar a profundidade
    for (int i = 0; i < nivel; i++)
        printf("    ");

    printf("[");

    for (int i = 0; i < node->numChaves; i++) {
        printf("%d", node->chaves[i]);

        if (i < node->numChaves - 1)
            printf(" | ");
    }

    printf("]\n");

    // Imprime os filhos recursivamente
    if (!node->folha) {
        for (int i = 0; i <= node->numChaves; i++) {
            print_node(node->filhos[i], nivel + 1);
        }
    }
}

void show_tree(BTree* tree) {
    if (tree == NULL || tree->root == NULL) {
        printf("Arvore vazia!\n");
        return;
    }

    print_node(tree->root, 0);
}

void free_node(Node* node) {
    if (node == NULL)
        return;

    // Libera todos os filhos
    if (!node->folha) {
        for (int i = 0; i <= node->numChaves; i++) {
            free_node(node->filhos[i]);
        }
    }

    // Libera o próprio nó
    free(node);
}

void free_tree(BTree* tree) {
    if (tree == NULL)
        return;

    free_node(tree->root);
    free(tree);
}

Node* search_node(Node* node, int data) {
    if (node == NULL)
        return NULL;

    int i = 0;

    // Encontra a posição onde a chave deveria estar
    while (i < node->numChaves && data > node->chaves[i]) {
        i++;
    }

    // Verifica se encontrou a chave
    if (i < node->numChaves && data == node->chaves[i]) {
        return node;
    }

    // Se chegou a uma folha, o valor não existe
    if (node->folha) {
        return NULL;
    }

    // Continua procurando no filho correspondente
    return search_node(node->filhos[i], data);
}

int is_full(Node* node){
    return node->numChaves == 6;
}

int is_empty(Node* node){
    return node->numChaves == 0;
}