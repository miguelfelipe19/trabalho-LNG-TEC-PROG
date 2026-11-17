/*
 * BATALHA NAVAL - PROGRAMA PRINCIPAL
 * Inicializa o sorteio e controla o menu.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "batalha_naval.h"

int main(void) {
    char entrada[80];
    char escolha;
    char extra;
    int opcao;

    srand((unsigned int)time(NULL));
    mostrarApresentacao();

    do {
        printf("\n==============================\n");
        printf("       BATALHA NAVAL\n");
        printf("==============================\n");
        printf("  1 - Jogar\n");
        printf("  2 - Ver historico\n");
        printf("  0 - Sair\n");
        printf("==============================\n");
        printf("Escolha: ");

        if (lerEntrada(entrada, sizeof(entrada)) == 0) {
            break;
        }

        opcao = -1;

        if (sscanf(entrada, " %c %c", &escolha, &extra) == 1 &&
            escolha >= '0' && escolha <= '2') {
            opcao = escolha - '0';
        }

        switch (opcao) {
            case 1:
                jogar();
                break;

            case 2:
                mostrarHistorico();
                break;

            case 0:
                printf("\nAte a proxima, capitao!\n");
                break;

            default:
                printf("\nOpcao invalida.\n");
                break;
        }
    } while (opcao != 0);

    return 0;
}
