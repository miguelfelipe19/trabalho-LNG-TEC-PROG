#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAMANHO_TABULEIRO 8    /* tabuleiro 8x8*/
#define QUANTIDADE_NAVIOS 3
#define MAX_TENTATIVAS 30

/* guarda as infos de UM navio */
typedef struct {
    int tamanho;    /* quantas casas o navio ocupa */
    int acertos;    /* quantas dessas casas ja foram atingidas */
    int afundado;   /* 0 = ainda flutuando, 1 = afundado */
} Navio;

typedef struct {
    char tabuleiro [TAMANHO_TABULEIRO][TAMANHO_TABULEIRO];
    char tiros [TAMANHO_TABULEIRO] [TAMANHO_TABULEIRO];
    int navioNaCasa [TAMANHO_TABULEIRO] [TAMANHO_TABULEIRO];
    Navio navios [QUANTIDADE_NAVIOS];
    int tentativas;
    int acertos;
} Jogo;

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

            if (horizontal && coluna + tamanho <= TAMANHO_TABULEIRO) {
                cabe = 1;
                for (posicao = 0; posicao < tamanho; posicao++) {
                    if (jogo->tabuleiro[linha][coluna + posicao] == 'N') {
                        cabe = 0;
                    }
                }
            } else if (!horizontal && linha + tamanho <= TAMANHO_TABULEIRO) {
                cabe = 1;
                for (posicao = 0; posicao < tamanho; posicao++) {
                    if (jogo->tabuleiro[linha + posicao][coluna] == 'N') {
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

void mostrarApresentacao(void) {
    printf("\n");
    printf("  ==========================================\n");
    printf("  ||                                      ||\n");
    printf("  ||          BATALHA   NAVAL             ||\n");
    printf("  ||                                      ||\n");
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
    getchar();
}

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
                casa = '~';   /* casa ainda nao atacada */
            }
            printf("%c ", casa);
        }
        printf("|\n");
    }
    printf("     -------------------\n");
    printf("\n Legenda: ~ nao tentado | O agua | X navio atingido\n");
}

int realizarAtaque(Jogo *jogo, int linha, int coluna) {
    int indice;

    jogo->tentativas++;
    if (jogo->tabuleiro[linha][coluna] == 'N') {
        indice = jogo->navioNaCasa[linha][coluna];
        jogo->tiros[linha][coluna] = 'X';
        jogo->tabuleiro[linha][coluna] = 'x';
        jogo->acertos++;
        jogo->navios[indice].acertos++;
        if (jogo->navios[indice].acertos == jogo->navios[indice].tamanho) {
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

int todosNaviosAfundados(Jogo *jogo) {
    int indice;
    for (indice = 0; indice < QUANTIDADE_NAVIOS; indice++) {
        if (jogo->navios[indice].afundado == 0) {
            return 0;
        }
    }
    return 1;
}

void jogar(void) {
    Jogo jogo;
    char entrada[80];
    char linhaLetra;
    int coluna;
    int linha;
    int lidos;
    int resultado;
    int venceu;
    FILE *arquivo;

    srand((unsigned int)time(NULL));
    inicializarJogo(&jogo);
    posicionarNavios(&jogo);
    venceu = 0;

    printf("\n==============================================\n");
    printf("  BATALHA NAVAL\n");
    printf("  Encontre os 7 pedacos de navio em ate %d tentativas.\n", MAX_TENTATIVAS);
    printf("==============================================\n");
    while (jogo.tentativas < MAX_TENTATIVAS && !todosNaviosAfundados(&jogo)) {
        mostrarTabuleiro(&jogo);
        printf("\n Tentativa %d de %d. Digite linha e coluna (ex.: A 3): ",
               jogo.tentativas + 1, MAX_TENTATIVAS);

        if (fgets(entrada, sizeof(entrada), stdin) == NULL) {
            printf("Entrada encerrada.\n");
            return;
        }
        lidos = sscanf(entrada, " %c %d", &linhaLetra, &coluna);
        if (lidos != 2) {
            printf("\n Entrada invalida. Use uma letra de A a H e um numero de 1 a 8.\n");
            continue;
        }

        if (linhaLetra >= 'a' && linhaLetra <= 'h') {
            linhaLetra = (char)(linhaLetra - 'a' + 'A');
        }
        linha = linhaLetra - 'A';
        coluna--;
        if (linha < 0 || linha >= TAMANHO_TABULEIRO ||
            coluna < 0 || coluna >= TAMANHO_TABULEIRO) {
            printf("\n Coordenada fora do tabuleiro. Tente novamente.\n");
            continue;
        }
        if (jogo.tiros[linha][coluna] != ' ') {
            printf("\n Voce ja tentou essa casa. Escolha outra.\n");
            continue;
        }

        resultado = realizarAtaque(&jogo, linha, coluna);
        if (resultado) {
            printf(" Total de acertos: %d de 7\n", jogo.acertos);
        }
    }

    venceu = todosNaviosAfundados(&jogo);
    mostrarTabuleiro(&jogo);
    printf("\n==============================================\n");
    if (venceu) {
        printf("  PARABENS! Voce venceu em %d tentativas!\n", jogo.tentativas);
    } else {
        printf("  FIM DE JOGO! Acabaram as tentativas.\n");
        printf("  Os navios restantes foram perdidos.\n");
    }
    printf("==============================================\n");

    arquivo = fopen("historico.txt", "a");
    if (arquivo == NULL) {
        printf("Aviso: nao foi possivel abrir o arquivo historico.txt.\n");
    } else {
        fprintf(arquivo, "Resultado: %s | Tentativas: %d | Acertos: %d\n",
                venceu ? "vitoria" : "fim de jogo", jogo.tentativas, jogo.acertos);
        fclose(arquivo);
    }
}

void mostrarHistorico(void) {
    FILE *arquivo;
    char linha[160];
    int encontrou;

    arquivo = fopen("historico.txt", "r");
    if (arquivo == NULL) {
        printf("\n Ainda nao ha partidas salvas.\n");
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
    if (!encontrou) {
        printf(" O historico esta vazio.\n");
    }
    fclose(arquivo);
}

int main(void) {
    char entrada[80];
    int opcao;

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

        if (fgets(entrada, sizeof(entrada), stdin) == NULL) {
            break;
        }
        if (sscanf(entrada, "%d", &opcao) != 1) {
            opcao = -1;
        }

        switch (opcao) {
            case 1: jogar(); break;
            case 2: mostrarHistorico(); break;
            case 0: printf("\nAte a proxima, capitao!\n"); break;
            default: printf("\nOpcao invalida.\n");
        }
    } while (opcao != 0);

    return 0;
}
