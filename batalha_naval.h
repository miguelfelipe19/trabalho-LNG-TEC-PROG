/*
 * BATALHA NAVAL - CABECALHO
 * Constantes, estruturas e declaracoes das funcoes.
 */

#ifndef BATALHA_NAVAL_H
#define BATALHA_NAVAL_H

/* ==========================================================
 * CONFIGURACOES DO JOGO
 * ========================================================== */

#define TAMANHO_TABULEIRO 8
#define QUANTIDADE_NAVIOS 3
#define MAX_TENTATIVAS 30

/* ==========================================================
 * ESTRUTURAS
 * ========================================================== */

/* Guarda as informacoes de um navio. */
typedef struct {
    int tamanho;
    int acertos;
    int afundado;
} Navio;

/* Guarda as informacoes da partida. */
typedef struct {
    char tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO];
    char tiros[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO];
    int navioNaCasa[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO];

    Navio navios[QUANTIDADE_NAVIOS];

    int tentativas;
    int acertos;
} Jogo;

/* ==========================================================
 * DECLARACOES DAS FUNCOES
 * ========================================================== */

int lerEntrada(char *entrada, int capacidade);

void inicializarJogo(Jogo *jogo);

void posicionarNavios(Jogo *jogo);

void mostrarApresentacao(void);

void mostrarTabuleiro(Jogo *jogo);

int realizarAtaque(Jogo *jogo, int linha, int coluna);

int todosNaviosAfundados(Jogo *jogo);

void limparTerminal(void);

void mostrarTelaDerrota(Jogo *jogo);

void jogar(void);

void mostrarHistorico(void);

#endif /* BATALHA_NAVAL_H */
