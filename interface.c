#define _POSIX_C_SOURCE 199309L

#include <stdio.h>
#include <string.h>
#include <time.h>
#include "interface.h"

#ifdef _WIN32
#include <windows.h>
#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif
#endif

#define LARGURA_CABECALHO 46

/* ---------- utilidades de terminal ---------- */

void prepararTerminal(void) {
#ifdef _WIN32
    /* liga as cores ANSI no terminal do Windows 10+ */
    HANDLE saida = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD modo = 0;
    if (saida != INVALID_HANDLE_VALUE && GetConsoleMode(saida, &modo)) {
        SetConsoleMode(saida, modo | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
    }
#endif
}

void limparTela(void) {
    printf("\033[2J\033[H");
    fflush(stdout);
}

void esperar(int milissegundos) {
    fflush(stdout);
#ifdef _WIN32
    Sleep((DWORD)milissegundos);
#else
    struct timespec pausa;
    pausa.tv_sec = milissegundos / 1000;
    pausa.tv_nsec = (long)(milissegundos % 1000) * 1000000L;
    nanosleep(&pausa, NULL);
#endif
}

/* le uma linha do teclado; devolve 0 se a entrada acabou (EOF) */
int lerLinha(char *destino, int tamanho) {
    int caractere;

    if (fgets(destino, tamanho, stdin) == NULL) {
        return 0;
    }
    if (strchr(destino, '\n') == NULL) {
        /* linha maior que o buffer: descarta o resto para nao atrapalhar a proxima leitura */
        while ((caractere = getchar()) != '\n' && caractere != EOF) {
        }
    }
    return 1;
}

void pausar(void) {
    char lixo[80];
    printf("\n  " COR_CINZA "Pressione ENTER para continuar..." COR_RESET);
    lerLinha(lixo, sizeof(lixo));
}

/* ---------- telas ---------- */

void mostrarCabecalho(const char *titulo) {
    int espacos;
    int i;

    espacos = (LARGURA_CABECALHO - (int)strlen(titulo)) / 2;
    if (espacos < 0) {
        espacos = 0;
    }

    printf("\n  " COR_CIANO);
    for (i = 0; i < LARGURA_CABECALHO; i++) {
        printf("=");
    }
    printf("\n  ");
    for (i = 0; i < espacos; i++) {
        printf(" ");
    }
    printf(COR_BRANCO COR_NEGRITO "%s" COR_RESET "\n  " COR_CIANO, titulo);
    for (i = 0; i < LARGURA_CABECALHO; i++) {
        printf("=");
    }
    printf(COR_RESET "\n");
}

void mostrarApresentacao(void) {
    int i;
    char entrada[80];
    const char *batalha[5] = {
        "####   ###  #####  ###  #     #   #  ### ",
        "#   # #   #   #   #   # #     #   # #   #",
        "####  #####   #   ##### #     ##### #####",
        "#   # #   #   #   #   # #     #   # #   #",
        "####  #   #   #   #   # ##### #   # #   #"
    };
    const char *naval[5] = {
        "#   #  ###  #   #  ###  #    ",
        "##  # #   # #   # #   # #    ",
        "# # # ##### #   # ##### #    ",
        "#  ## #   #  # #  #   # #    ",
        "#   # #   #   #   #   # #####"
    };

    limparTela();
    printf("\n");
    for (i = 0; i < 5; i++) {
        printf("   " COR_CIANO COR_NEGRITO "%s" COR_RESET "\n", batalha[i]);
        esperar(90);
    }
    printf("\n");
    for (i = 0; i < 5; i++) {
        printf("   " COR_AMARELO COR_NEGRITO "%s" COR_RESET "\n", naval[i]);
        esperar(90);
    }

    printf("\n" COR_BRANCO);
    printf("                      |    |    |\n");
    printf("                     )_)  )_)  )_)\n");
    printf("                    )___))___))___)\\\n");
    printf("                   )____)____)_____)\\\\\n");
    printf("                 _____|____|____|____\\\\\\__\n");
    printf(COR_AZUL "        ---------" COR_BRANCO "\\                   /" COR_AZUL "---------\n");
    printf("          ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");
    printf("            ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n" COR_RESET);

    printf("\n  " COR_CINZA "Projeto academico - Linguagem e Tecnicas de Programacao" COR_RESET "\n");
    printf("\n  Preparando a frota  [");
    for (i = 0; i < 30; i++) {
        printf(COR_VERDE "#" COR_RESET);
        esperar(45);
    }
    printf("] 100%%\n");
    esperar(300);

    printf("\n  " COR_AMARELO "Pressione ENTER para zarpar..." COR_RESET);
    lerLinha(entrada, sizeof(entrada));
}

void mostrarMenu(void) {
    mostrarCabecalho("B A T A L H A   N A V A L");
    printf("\n  " COR_AZUL "~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~" COR_RESET "\n\n");
    printf("      " COR_AMARELO "[1]" COR_RESET "  Jogar\n");
    printf("      " COR_AMARELO "[2]" COR_RESET "  Como jogar\n");
    printf("      " COR_AMARELO "[3]" COR_RESET "  Ver historico de partidas\n");
    printf("      " COR_AMARELO "[0]" COR_RESET "  Sair\n");
    printf("\n  " COR_AZUL "~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~" COR_RESET "\n");
    printf("\n  Escolha uma opcao: ");
}

void mostrarMenuDificuldade(void) {
    mostrarCabecalho("ESCOLHA A DIFICULDADE");
    printf("\n      " COR_VERDE "[1]" COR_RESET "  Facil    - %d tentativas\n", TENTATIVAS_FACIL);
    printf("      " COR_AMARELO "[2]" COR_RESET "  Medio    - %d tentativas\n", TENTATIVAS_MEDIO);
    printf("      " COR_VERMELHO "[3]" COR_RESET "  Dificil  - %d tentativas\n", TENTATIVAS_DIFICIL);
    printf("      " COR_CINZA "[0]" COR_RESET "  Voltar\n");
    printf("\n  Escolha: ");
}

void mostrarComoJogar(void) {
    limparTela();
    mostrarCabecalho("COMO JOGAR");
    printf("\n  Uma frota inimiga esta escondida no tabuleiro 8x8:\n\n");
    printf("      " COR_BRANCO "Cruzador" COR_RESET "   - ocupa 3 casas\n");
    printf("      " COR_BRANCO "Submarino" COR_RESET "  - ocupa 2 casas\n");
    printf("      " COR_BRANCO "Destroyer" COR_RESET "  - ocupa 2 casas\n");
    printf("\n  Os navios ficam na horizontal ou na vertical e sao\n");
    printf("  posicionados de forma aleatoria a cada partida.\n");
    printf("\n  " COR_AMARELO "Como atirar:" COR_RESET " digite a LINHA (letra) e a COLUNA (numero),\n");
    printf("  por exemplo " COR_BRANCO "A 3" COR_RESET " ou " COR_BRANCO "h7" COR_RESET ". Digite " COR_BRANCO "0" COR_RESET " para desistir.\n");
    printf("\n  " COR_AMARELO "Legenda do tabuleiro:" COR_RESET "\n");
    printf("      " COR_AZUL "~" COR_RESET "  casa ainda nao atacada\n");
    printf("      " COR_BRANCO "O" COR_RESET "  agua (voce errou)\n");
    printf("      " COR_VERMELHO COR_NEGRITO "X" COR_RESET "  parte de navio atingida\n");
    printf("      " COR_VERMELHO "#" COR_RESET "  navio afundado\n");
    printf("\n  " COR_AMARELO "Objetivo:" COR_RESET " afundar os 3 navios (7 partes) antes de\n");
    printf("  acabarem as tentativas. Quanto menos tiros, melhor a patente!\n");
}

void mostrarDespedida(void) {
    limparTela();
    mostrarCabecalho("ATE A PROXIMA, CAPITAO!");
    printf("\n  " COR_AZUL "~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~" COR_RESET "\n");
    printf("  Obrigado por jogar Batalha Naval!\n\n");
}

/* ---------- tela da partida ---------- */

void mostrarTabuleiro(const Jogo *jogo, int revelar) {
    int linha;
    int coluna;
    int indice;
    char tiro;

    printf("\n        ");
    for (coluna = 0; coluna < TAMANHO_TABULEIRO; coluna++) {
        printf(COR_AMARELO "%d  " COR_RESET, coluna + 1);
    }
    printf("\n      +-------------------------+\n");

    for (linha = 0; linha < TAMANHO_TABULEIRO; linha++) {
        printf("   " COR_AMARELO "%c" COR_RESET "  | ", 'A' + linha);
        for (coluna = 0; coluna < TAMANHO_TABULEIRO; coluna++) {
            tiro = jogo->tiros[linha][coluna];
            if (tiro == 'X') {
                indice = jogo->navioNaCasa[linha][coluna];
                if (jogo->navios[indice].afundado) {
                    printf(COR_VERMELHO "#  " COR_RESET);
                } else {
                    printf(COR_VERMELHO COR_NEGRITO "X  " COR_RESET);
                }
            } else if (tiro == 'O') {
                printf(COR_BRANCO "O  " COR_RESET);
            } else if (revelar && jogo->tabuleiro[linha][coluna] == 'N') {
                printf(COR_AMARELO "N  " COR_RESET);
            } else {
                printf(COR_AZUL "~  " COR_RESET);
            }
        }
        printf("|\n");
    }
    printf("      +-------------------------+\n");

    printf("\n  " COR_AZUL "~" COR_RESET " nao atacado   " COR_BRANCO "O" COR_RESET " agua   "
           COR_VERMELHO COR_NEGRITO "X" COR_RESET " acerto   " COR_VERMELHO "#" COR_RESET " afundado");
    if (revelar) {
        printf("   " COR_AMARELO "N" COR_RESET " navio escondido");
    }
    printf("\n");
}

void mostrarPainel(const Jogo *jogo) {
    int indice;
    int parte;
    int cheios;
    int i;
    const char *cor;

    cheios = jogo->tentativas * 20 / jogo->maxTentativas;
    if (jogo->tentativas * 100 / jogo->maxTentativas < 60) {
        cor = COR_VERDE;
    } else if (jogo->tentativas * 100 / jogo->maxTentativas < 85) {
        cor = COR_AMARELO;
    } else {
        cor = COR_VERMELHO;
    }

    printf("\n  Dificuldade: " COR_BRANCO "%s" COR_RESET "\n", nomeDificuldade(jogo->dificuldade));
    printf("  Tentativas : %d/%d  [%s", jogo->tentativas, jogo->maxTentativas, cor);
    for (i = 0; i < 20; i++) {
        printf("%c", i < cheios ? '#' : '-');
    }
    printf(COR_RESET "]\n");
    printf("  Acertos    : %d/%d\n", jogo->acertos, TOTAL_PARTES_NAVIOS);

    printf("\n  " COR_CIANO "Frota inimiga:" COR_RESET "\n");
    for (indice = 0; indice < QUANTIDADE_NAVIOS; indice++) {
        printf("    %-10s (%d)  [", jogo->navios[indice].nome, jogo->navios[indice].tamanho);
        for (parte = 0; parte < jogo->navios[indice].tamanho; parte++) {
            if (parte < jogo->navios[indice].acertos) {
                printf(COR_VERMELHO "X" COR_RESET);
            } else {
                printf(COR_CINZA "." COR_RESET);
            }
            if (parte < jogo->navios[indice].tamanho - 1) {
                printf(" ");
            }
        }
        printf("]");
        for (parte = jogo->navios[indice].tamanho; parte < 3; parte++) {
            printf("  ");   /* alinha a coluna de status */
        }
        printf("  ");
        if (jogo->navios[indice].afundado) {
            printf(COR_VERMELHO "AFUNDADO" COR_RESET "\n");
        } else {
            printf(COR_VERDE "navegando" COR_RESET "\n");
        }
    }
}

void mostrarResultado(const Jogo *jogo, int venceu) {
    int estrelas;
    int i;
    const char *patente;

    if (venceu) {
        if (jogo->tentativas * 100 / jogo->maxTentativas <= 45) {
            estrelas = 3;
            patente = "Almirante";
        } else if (jogo->tentativas * 100 / jogo->maxTentativas <= 70) {
            estrelas = 2;
            patente = "Capitao";
        } else {
            estrelas = 1;
            patente = "Marinheiro";
        }

        printf("\n  " COR_VERDE COR_NEGRITO);
        printf("+------------------------------------------+\n");
        printf("  |        V I T O R I A !   (^_^)           |\n");
        printf("  +------------------------------------------+" COR_RESET "\n");
        printf("\n  Voce afundou toda a frota em " COR_BRANCO "%d" COR_RESET " tentativas!\n", jogo->tentativas);
        printf("  Patente conquistada: " COR_AMARELO "%s" COR_RESET "  ", patente);
        for (i = 0; i < 3; i++) {
            printf(i < estrelas ? COR_AMARELO "[*]" COR_RESET : COR_CINZA "[ ]" COR_RESET);
        }
        printf("\n");
    } else {
        printf("\n  " COR_VERMELHO COR_NEGRITO);
        printf("+------------------------------------------+\n");
        printf("  |        F I M   D E   J O G O   (T_T)     |\n");
        printf("  +------------------------------------------+" COR_RESET "\n");
        printf("\n  As tentativas acabaram. Voce acertou " COR_BRANCO "%d" COR_RESET " de %d partes de navio.\n",
               jogo->acertos, TOTAL_PARTES_NAVIOS);
        printf("  Os navios que escaparam estao marcados com " COR_AMARELO "N" COR_RESET " no tabuleiro.\n");
    }
}
