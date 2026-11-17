#include <stdlib.h>
#include <string.h>
#include "jogo.h"

void inicializarJogo(Jogo *jogo, int dificuldade) {
    int linha;
    int coluna;
    int indice;
    int tamanhos[QUANTIDADE_NAVIOS] = {3, 2, 2};
    const char *nomes[QUANTIDADE_NAVIOS] = {"Cruzador", "Submarino", "Destroyer"};

    jogo->tentativas = 0;
    jogo->acertos = 0;
    jogo->dificuldade = dificuldade;
    jogo->ultimoNavio = -1;

    switch (dificuldade) {
        case DIFICULDADE_FACIL:   jogo->maxTentativas = TENTATIVAS_FACIL; break;
        case DIFICULDADE_DIFICIL: jogo->maxTentativas = TENTATIVAS_DIFICIL; break;
        default:                  jogo->maxTentativas = TENTATIVAS_MEDIO; break;
    }

    for (linha = 0; linha < TAMANHO_TABULEIRO; linha++) {
        for (coluna = 0; coluna < TAMANHO_TABULEIRO; coluna++) {
            jogo->tabuleiro[linha][coluna] = '~';
            jogo->tiros[linha][coluna] = ' ';
            jogo->navioNaCasa[linha][coluna] = -1;
        }
    }

    for (indice = 0; indice < QUANTIDADE_NAVIOS; indice++) {
        strncpy(jogo->navios[indice].nome, nomes[indice], TAMANHO_NOME_NAVIO - 1);
        jogo->navios[indice].nome[TAMANHO_NOME_NAVIO - 1] = '\0';
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

int realizarAtaque(Jogo *jogo, int linha, int coluna) {
    int indice;

    jogo->tentativas++;
    if (jogo->tabuleiro[linha][coluna] == 'N') {
        indice = jogo->navioNaCasa[linha][coluna];
        jogo->ultimoNavio = indice;
        jogo->tiros[linha][coluna] = 'X';
        jogo->tabuleiro[linha][coluna] = 'x';
        jogo->acertos++;
        jogo->navios[indice].acertos++;
        if (jogo->navios[indice].acertos == jogo->navios[indice].tamanho) {
            jogo->navios[indice].afundado = 1;
            return RESULTADO_AFUNDOU;
        }
        return RESULTADO_ACERTO;
    }

    jogo->tiros[linha][coluna] = 'O';
    return RESULTADO_AGUA;
}

int todosNaviosAfundados(const Jogo *jogo) {
    int indice;
    for (indice = 0; indice < QUANTIDADE_NAVIOS; indice++) {
        if (jogo->navios[indice].afundado == 0) {
            return 0;
        }
    }
    return 1;
}

int contarNaviosRestantes(const Jogo *jogo) {
    int indice;
    int restantes = 0;
    for (indice = 0; indice < QUANTIDADE_NAVIOS; indice++) {
        if (jogo->navios[indice].afundado == 0) {
            restantes++;
        }
    }
    return restantes;
}

const char *nomeDificuldade(int dificuldade) {
    switch (dificuldade) {
        case DIFICULDADE_FACIL:   return "Facil";
        case DIFICULDADE_DIFICIL: return "Dificil";
        default:                  return "Medio";
    }
}
