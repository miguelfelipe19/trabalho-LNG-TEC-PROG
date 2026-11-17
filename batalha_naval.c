/*
 * BATALHA NAVAL - IMPLEMENTACAO
 * Funcoes que fazem o jogo funcionar.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "batalha_naval.h"

/* ==========================================================
 * LEITURA DE ENTRADAS
 * ========================================================== */

/*
 * Retorna:
 *  1: leitura realizada;
 *  0: fim da entrada;
 * -1: entrada muito longa.
 */
int lerEntrada(char *entrada, int capacidade) {
    int caractere;

    if (fgets(entrada, capacidade, stdin) == NULL) {
        return 0;
    }

    if (strchr(entrada, '\n') == NULL && !feof(stdin)) {
        caractere = getchar();

        if (caractere != '\n' && caractere != EOF) {
            while ((caractere = getchar()) != '\n' &&
                   caractere != EOF) {
                /* Descarta o restante da entrada. */
            }

            entrada[0] = '\0';
            return -1;
        }
    }

    return 1;
}

/* ==========================================================
 * INICIALIZACAO DO JOGO
 * ========================================================== */

void inicializarJogo(Jogo *jogo) {
    int linha;
    int coluna;
    int indice;
    int tamanhos[QUANTIDADE_NAVIOS] = {3, 2, 2};

    jogo->tentativas = 0;
    jogo->acertos = 0;

    for (linha = 0; linha < TAMANHO_TABULEIRO; linha++) {
        for (coluna = 0; coluna < TAMANHO_TABULEIRO; coluna++) {
            jogo->tabuleiro[linha][coluna] = '~';
            jogo->tiros[linha][coluna] = ' ';
            jogo->navioNaCasa[linha][coluna] = -1;
        }
    }

    for (indice = 0; indice < QUANTIDADE_NAVIOS; indice++) {
        jogo->navios[indice].tamanho = tamanhos[indice];
        jogo->navios[indice].acertos = 0;
        jogo->navios[indice].afundado = 0;
    }
}

/* ==========================================================
 * POSICIONAMENTO DOS NAVIOS
 * ========================================================== */

void posicionarNavios(Jogo *jogo) {
    int indice;
    int tamanho;
    int linha;
    int coluna;
    int horizontal;
    int cabe;
    int posicao;

    for (indice = 0; indice < QUANTIDADE_NAVIOS; indice++) {
        tamanho = jogo->navios[indice].tamanho;
        cabe = 0;

        while (!cabe) {
            horizontal = rand() % 2;
            linha = rand() % TAMANHO_TABULEIRO;
            coluna = rand() % TAMANHO_TABULEIRO;

            if (horizontal &&
                coluna + tamanho <= TAMANHO_TABULEIRO) {
                cabe = 1;

                for (posicao = 0; posicao < tamanho; posicao++) {
                    if (jogo->tabuleiro[linha][coluna + posicao]
                        == 'N') {
                        cabe = 0;
                    }
                }
            } else if (!horizontal &&
                       linha + tamanho <= TAMANHO_TABULEIRO) {
                cabe = 1;

                for (posicao = 0; posicao < tamanho; posicao++) {
                    if (jogo->tabuleiro[linha + posicao][coluna]
                        == 'N') {
                        cabe = 0;
                    }
                }
            }
        }

        for (posicao = 0; posicao < tamanho; posicao++) {
            if (horizontal) {
                jogo->tabuleiro[linha][coluna + posicao] = 'N';
                jogo->navioNaCasa[linha][coluna + posicao] = indice;
            } else {
                jogo->tabuleiro[linha + posicao][coluna] = 'N';
                jogo->navioNaCasa[linha + posicao][coluna] = indice;
            }
        }
    }
}

/* ==========================================================
 * APRESENTACAO
 * ========================================================== */

void mostrarApresentacao(void) {
    char entrada[80];

    printf("\n");
    printf("  ==========================================\n");
    printf("             BATALHA NAVAL\n");
    printf("  ==========================================\n");
    printf("\n");
    printf("                   |\\\n");
    printf("                   | \\\n");
    printf("              _____|__\\_____\n");
    printf("              \\___________/\n");
    printf("      ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    printf("\n");
    printf("   Projeto de Linguagem e Tecnicas de Programacao\n");
    printf("\n   Pressione ENTER para comecar...");

    lerEntrada(entrada, sizeof(entrada));
}

/* ==========================================================
 * EXIBICAO DO TABULEIRO
 * ========================================================== */

void mostrarTabuleiro(Jogo *jogo) {
    int linha;
    int coluna;
    char casa;

    printf("\n       1 2 3 4 5 6 7 8\n");
    printf("     -------------------\n");

    for (linha = 0; linha < TAMANHO_TABULEIRO; linha++) {
        printf("   %c | ", 'A' + linha);

        for (coluna = 0; coluna < TAMANHO_TABULEIRO; coluna++) {
            casa = jogo->tiros[linha][coluna];

            if (casa == ' ') {
                casa = '~';
            }

            printf("%c ", casa);
        }

        printf("|\n");
    }

    printf("     -------------------\n");
    printf("\n Legenda: ~ nao tentado | O agua | X navio atingido\n");
}

/* ==========================================================
 * ATAQUES
 * ========================================================== */

int realizarAtaque(Jogo *jogo, int linha, int coluna) {
    int indice;

    jogo->tentativas++;

    if (jogo->tabuleiro[linha][coluna] == 'N') {
        indice = jogo->navioNaCasa[linha][coluna];

        jogo->tiros[linha][coluna] = 'X';
        jogo->tabuleiro[linha][coluna] = 'x';
        jogo->acertos++;
        jogo->navios[indice].acertos++;

        if (jogo->navios[indice].acertos ==
            jogo->navios[indice].tamanho) {
            jogo->navios[indice].afundado = 1;
            printf("\n >>> BOOM! Voce afundou um navio! <<<\n");
        } else {
            printf("\n >>> Acertou um navio! <<<\n");
        }

        return 1;
    }

    jogo->tiros[linha][coluna] = 'O';
    printf("\n >>> Agua! <<<\n");

    return 0;
}

/* ==========================================================
 * VERIFICACAO DA VITORIA
 * ========================================================== */

int todosNaviosAfundados(Jogo *jogo) {
    int indice;

    for (indice = 0; indice < QUANTIDADE_NAVIOS; indice++) {
        if (jogo->navios[indice].afundado == 0) {
            return 0;
        }
    }

    return 1;
}

/* ==========================================================
 * SEPARACAO VISUAL DAS TELAS
 * ========================================================== */

/* Separa as telas sem depender de comandos do sistema. */
void limparTerminal(void) {
    printf("\n\n--------------------------------------------------\n");
}

/* ==========================================================
 * TELA DE DERROTA
 * ========================================================== */

void mostrarTelaDerrota(Jogo *jogo) {
    char entrada[80];

    limparTerminal();

    printf("\n");
    printf("  ==========================================\n");
    printf("              VOCE PERDEU!\n");
    printf("  ==========================================\n");
    printf("\n");
    printf("                      |\n");
    printf("                      |   \\\n");
    printf("                  ____|____\\__\n");
    printf("                  \\__________/\n");
    printf("      ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    printf("        ~~~~~~~  GLUB  GLUB  ~~~~~~~~\n");
    printf("            ~~~~~~~~~~~~~~~~~~~~\n");
    printf("\n");
    printf("   Suas %d tentativas acabaram.\n", MAX_TENTATIVAS);
    printf("   Voce acertou %d de 7 pedacos de navio.\n",
           jogo->acertos);
    printf("   A frota inimiga escapou, capitao...\n");
    printf("\n   Pressione ENTER para voltar ao menu...");

    lerEntrada(entrada, sizeof(entrada));
    limparTerminal();
}

/* ==========================================================
 * CONTROLE DA PARTIDA
 * ========================================================== */

void jogar(void) {
    Jogo jogo;
    char entrada[80];
    char linhaLetra;
    char colunaLetra;
    char extra;
    int coluna;
    int linha;
    int lidos;
    int resultado;
    int venceu;
    int salvou;
    FILE *arquivo;

    inicializarJogo(&jogo);
    posicionarNavios(&jogo);

    printf("\n==============================================\n");
    printf("  BATALHA NAVAL\n");
    printf("  Encontre os 7 pedacos de navio em ate %d tentativas.\n",
           MAX_TENTATIVAS);
    printf("==============================================\n");

    while (jogo.tentativas < MAX_TENTATIVAS &&
           !todosNaviosAfundados(&jogo)) {
        mostrarTabuleiro(&jogo);

        printf("\n Tentativa %d de %d.",
               jogo.tentativas + 1, MAX_TENTATIVAS);
        printf(" Digite linha e coluna (ex.: A 3): ");

        if (lerEntrada(entrada, sizeof(entrada)) == 0) {
            printf("\nEntrada encerrada.\n");
            return;
        }

        lidos = sscanf(entrada, " %c %c %c",
                       &linhaLetra, &colunaLetra, &extra);

        if (lidos != 2) {
            printf("\n Entrada invalida.");
            printf(" Use uma letra de A a H e um numero de 1 a 8.\n");
            continue;
        }

        if (linhaLetra >= 'a' && linhaLetra <= 'h') {
            linhaLetra = (char)(linhaLetra - 'a' + 'A');
        }

        linha = linhaLetra - 'A';
        coluna = colunaLetra - '1';

        if (linha < 0 || linha >= TAMANHO_TABULEIRO ||
            coluna < 0 || coluna >= TAMANHO_TABULEIRO) {
            printf("\n Coordenada fora do tabuleiro.");
            printf(" Tente novamente.\n");
            continue;
        }

        if (jogo.tiros[linha][coluna] != ' ') {
            printf("\n Voce ja tentou essa casa.");
            printf(" Escolha outra.\n");
            continue;
        }

        resultado = realizarAtaque(&jogo, linha, coluna);

        if (resultado) {
            printf(" Total de acertos: %d de 7\n", jogo.acertos);
        }
    }

    venceu = todosNaviosAfundados(&jogo);

    /* Salva o resultado da partida no historico. */
    salvou = 0;
    arquivo = fopen("historico.txt", "a");

    if (arquivo != NULL) {
        salvou = fprintf(
            arquivo,
            "Resultado: %s | Tentativas: %d | Acertos: %d\n",
            venceu ? "vitoria" : "fim de jogo",
            jogo.tentativas,
            jogo.acertos
        ) >= 0;

        if (fclose(arquivo) != 0) {
            salvou = 0;
        }
    }

    if (venceu) {
        mostrarTabuleiro(&jogo);

        printf("\n==============================================\n");
        printf("  PARABENS! Voce venceu em %d tentativas!\n",
               jogo.tentativas);
        printf("==============================================\n");
    } else {
        mostrarTelaDerrota(&jogo);
    }

    if (!salvou) {
        printf("\nAviso: nao foi possivel salvar o historico.txt.\n");
    }
}

/* ==========================================================
 * CONSULTA DO HISTORICO
 * ========================================================== */

void mostrarHistorico(void) {
    FILE *arquivo;
    char linha[160];
    int encontrou;

    arquivo = fopen("historico.txt", "r");

    if (arquivo == NULL) {
        if (errno == ENOENT) {
            printf("\n Ainda nao ha partidas salvas.\n");
        } else {
            printf("\n Erro ao abrir o arquivo historico.txt.\n");
        }

        return;
    }

    encontrou = 0;

    printf("\n==============================================\n");
    printf("  HISTORICO DE PARTIDAS\n");
    printf("==============================================\n");

    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        printf(" %s", linha);
        encontrou = 1;
    }

    if (ferror(arquivo)) {
        printf("\n Erro ao ler o arquivo historico.txt.\n");
    } else if (!encontrou) {
        printf(" O historico esta vazio.\n");
    }

    if (fclose(arquivo) != 0) {
        printf("\n Erro ao fechar o arquivo historico.txt.\n");
    }
}
