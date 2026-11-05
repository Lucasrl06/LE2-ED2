#include <stdlib.h>
#include <stdio.h>

#define ORDEM 7

typedef struct Node {
    int chaves[ORDEM - 1]; // chaves são o número de elementos que cabe dentro de um nó
    struct Node *filhos[ORDEM]; // ordem 7 = 7 filhos
    int numChaves;
    int folha;
} Node;

typedef struct BTree {
    Node *root;
} BTree;

BTree* create_tree(){}
//cria a arvore :D


void insert(BTree* tree, Node* node, int data){
    // verifica se a árvore está vazia
    if(tree->root == NULL){
        tree->root = create_node();
        tree->root->folha = 1; // indica que esse nó não possui filhos
        tree->root->chaves[0] = data;
        tree->root->numChaves = 1;
        return;
    }

    Node* root = tree->root; // guarda a raiz atual pra ver se ela está cheia

    if(is_full(root)){ // se a raiz ta cheia, precisa criar uma nova raiz
        Node* newRoot = create_node();

        newRoot->folha = 0; // a nova raiz vai ser um nó interno, pq vai ter filhos
        newRoot->filhos[0] = root; // coloca a raiz antiga como primeiro filho

        split_node(newRoot,); // divide a raiz antiga
        tree->root = newRoot; // atualiza a raiz da árvore
    }

    insert_non_full(tree->root, data); // inicia a inserção a partir da raiz atual
}

void insert_non_full(Node* node, int data){

    int i = node->numChaves - 1;

    if(node->folha){ // se for folha, o valor será inserido diretamente nesse no

        while(i >= 0 && node->chaves[i] > data){
            node->chaves[i+1] = node->chaves[i]; // desloca as chaves maiores para abrir espaço
            i--;
        }

        node->chaves[i+1] = data; // insere o valor na posição correta
        node->numChaves++; // atualiza a quantidade de chaves

        return;
    }

    // se não for folha, precisa encontrar em qual filho o valor deve entrar
    while(i >= 0 && node->chaves[i] > data){
        i--;
    }

    i++; // ajusta o índice para acessar o filho correspondente

    if(is_full(node->filhos[i])){ 
        split_node(node,); // divide o filho

        if(data > node->chaves[i]){
            i++; // se o valor for maior que a mediana, vai pro filho da direita
        }
    }

    insert_non_full(node->filhos[i], data);
}

void split_node(Node* node){}

//quando um nó chega no tamanho limite(6), ele terá que splitar, pra isso, deve-se pegar

void remove_data(){}
// vai usar search_node pra procurar o nó do dado que procura, se achar, 
//percorre o vetor chaves com for e if pra identificar o valor certo
int recover_data(){}

BTree* show_tree(){}

Node* search_node(){}

int is_full(Node* node){
    return node->numChaves == 6;
}

int is_empty(Node* node){
    return node->numChaves == 0;
}