#include <stdio.h>
#include "interface.h"
#include "partida.h"
#include "historico.h"

int main(void) {
    char entrada[80];
    int opcao;

    prepararTerminal();
    mostrarApresentacao();

    do {
        limparTela();
        mostrarMenu();

        if (!lerLinha(entrada, sizeof(entrada))) {
            break;
        }
        if (sscanf(entrada, "%d", &opcao) != 1) {
            opcao = -1;
        }

        switch (opcao) {
            case 1:
                jogar();
                break;
            case 2:
                mostrarComoJogar();
                pausar();
                break;
            case 3:
                mostrarHistorico();
                pausar();
                break;
            case 0:
                mostrarDespedida();
                break;
            default:
                printf("\n  " COR_VERMELHO "Opcao invalida. Escolha 0, 1, 2 ou 3." COR_RESET "\n");
                pausar();
        }
    } while (opcao != 0);

    return 0;
}
