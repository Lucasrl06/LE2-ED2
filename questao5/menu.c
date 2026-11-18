#include <stdlib.h>
#include <stdio.h>
#include "../questao5/TAD_btree.h"

int main(void) {
    int opcao, valor;

    // Inicializar a B-tree
    // Gerar e inserir os 100 números aleatórios
    create_tree();


    do {
        printf("\n===== MENU B-TREE =====\n");
        printf("1 - Inserir\n");
        printf("2 - Remover\n");
        printf("3 - Recuperar dados\n");
        printf("4 - Imprimir arvore\n");
        printf("0 - Sair\n");
        printf("Opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                printf("Valor para inserir: ");
                scanf("%d", &valor);
                insert();
                break;

            case 2:
                printf("Valor para remover: ");
                scanf("%d", &valor);
                int dado = remove_data();
                printf("%d foi removido", dado);
                break;

            case 3:
                printf("Valor para buscar: ");
                scanf("%d", &valor);
                search_node();
                break;

            case 4:
                show_tree();
                break;

            case 0:
                printf("Encerrando...\n");
                break;

            default:
                printf("Opcao invalida!\n");
        }

    } while (opcao != 0);

    return 0;
}