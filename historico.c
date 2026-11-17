#include <stdio.h>
#include <string.h>
#include <time.h>
#include "historico.h"
#include "interface.h"

int salvarPartida(const Jogo *jogo, int venceu) {
    FILE *arquivo;
    char data[32];
    time_t agora;

    arquivo = fopen(ARQUIVO_HISTORICO, "a");
    if (arquivo == NULL) {
        return 0;
    }

    agora = time(NULL);
    strftime(data, sizeof(data), "%d/%m/%Y %H:%M", localtime(&agora));

    fprintf(arquivo, "%s | %-7s | %-8s | Tentativas: %d/%d | Acertos: %d/%d\n",
            data, nomeDificuldade(jogo->dificuldade), venceu ? "vitoria" : "derrota",
            jogo->tentativas, jogo->maxTentativas, jogo->acertos, TOTAL_PARTES_NAVIOS);
    fclose(arquivo);
    return 1;
}

void mostrarHistorico(void) {
    FILE *arquivo;
    char linha[200];
    int total;
    int vitorias;

    limparTela();
    mostrarCabecalho("HISTORICO DE PARTIDAS");

    arquivo = fopen(ARQUIVO_HISTORICO, "r");
    if (arquivo == NULL) {
        printf("\n  " COR_AMARELO "Ainda nao ha partidas salvas. Jogue uma para comecar!" COR_RESET "\n");
        return;
    }

    total = 0;
    vitorias = 0;
    printf("\n");
    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        total++;
        if (strstr(linha, "vitoria") != NULL) {
            vitorias++;
            printf("  " COR_VERDE "%s" COR_RESET, linha);
        } else if (strstr(linha, "derrota") != NULL || strstr(linha, "fim de jogo") != NULL) {
            printf("  " COR_VERMELHO "%s" COR_RESET, linha);
        } else {
            printf("  %s", linha);   /* linhas no formato antigo */
        }
    }
    fclose(arquivo);

    if (total == 0) {
        printf("  " COR_AMARELO "O historico esta vazio." COR_RESET "\n");
        return;
    }
    printf("\n  Partidas: " COR_BRANCO "%d" COR_RESET "  |  Vitorias: " COR_VERDE "%d" COR_RESET
           "  |  Derrotas: " COR_VERMELHO "%d" COR_RESET "\n", total, vitorias, total - vitorias);
}
