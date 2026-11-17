#ifndef JOGO_H
#define JOGO_H

#define TAMANHO_TABULEIRO 8    /* tabuleiro 8x8 */
#define QUANTIDADE_NAVIOS 3
#define TOTAL_PARTES_NAVIOS 7  /* 3 + 2 + 2 casas */
#define TAMANHO_NOME_NAVIO 16

/* niveis de dificuldade (mudam so o numero maximo de tentativas) */
#define DIFICULDADE_FACIL 1
#define DIFICULDADE_MEDIO 2
#define DIFICULDADE_DIFICIL 3

#define TENTATIVAS_FACIL 40
#define TENTATIVAS_MEDIO 30
#define TENTATIVAS_DIFICIL 20

/* retornos de realizarAtaque */
#define RESULTADO_AGUA 0
#define RESULTADO_ACERTO 1
#define RESULTADO_AFUNDOU 2

/* guarda as infos de UM navio */
typedef struct {
    char nome[TAMANHO_NOME_NAVIO];
    int tamanho;    /* quantas casas o navio ocupa */
    int acertos;    /* quantas dessas casas ja foram atingidas */
    int afundado;   /* 0 = ainda flutuando, 1 = afundado */
} Navio;

typedef struct {
    char tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO];   /* 'N' = navio, '~' = agua */
    char tiros[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO];       /* ' ' = nao tentado, 'O' = agua, 'X' = acerto */
    int navioNaCasa[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO];  /* indice do navio na casa (-1 = nenhum) */
    Navio navios[QUANTIDADE_NAVIOS];
    int tentativas;
    int acertos;
    int maxTentativas;
    int dificuldade;
    int ultimoNavio;   /* indice do ultimo navio atingido (para as mensagens) */
} Jogo;

void inicializarJogo(Jogo *jogo, int dificuldade);
void posicionarNavios(Jogo *jogo);
int realizarAtaque(Jogo *jogo, int linha, int coluna);
int todosNaviosAfundados(const Jogo *jogo);
int contarNaviosRestantes(const Jogo *jogo);
const char *nomeDificuldade(int dificuldade);

#endif
