#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "partida.h"
#include "jogo.h"
#include "interface.h"
#include "historico.h"

/* retornos de lerCoordenada */
#define COORD_OK 0
#define COORD_FORMATO_INVALIDO 1
#define COORD_FORA_DO_TABULEIRO 2
#define COORD_DESISTIR 3

static int escolherDificuldade(void) {
    char entrada[80];
    int opcao;

    while (1) {
        limparTela();
        mostrarMenuDificuldade();

        if (!lerLinha(entrada, sizeof(entrada))) {
            return 0;
        }
        if (sscanf(entrada, "%d", &opcao) != 1) {
            opcao = -1;
        }
        if (opcao >= 0 && opcao <= 3) {
            return opcao;
        }
        printf("\n  " COR_VERMELHO "Opcao invalida. Escolha de 0 a 3." COR_RESET "\n");
        pausar();
    }
}

static int lerCoordenada(const char *entrada, int *linha, int *coluna) {
    char letra;
    int numero;
    int lidos;

    lidos = sscanf(entrada, " %c %d", &letra, &numero);
    if (lidos >= 1 && letra == '0') {
        return COORD_DESISTIR;
    }
    if (lidos != 2) {
        return COORD_FORMATO_INVALIDO;
    }

    if (letra >= 'a' && letra <= 'h') {
        letra = (char)(letra - 'a' + 'A');
    }
    *linha = letra - 'A';
    *coluna = numero - 1;

    if (*linha < 0 || *linha >= TAMANHO_TABULEIRO ||
        *coluna < 0 || *coluna >= TAMANHO_TABULEIRO) {
        return COORD_FORA_DO_TABULEIRO;
    }
    return COORD_OK;
}

void jogar(void) {
    Jogo jogo;
    char entrada[80];
    char mensagem[200];
    int dificuldade;
    int linha;
    int coluna;
    int status;
    int resultado;
    int venceu;
    int desistiu;

    dificuldade = escolherDificuldade();
    if (dificuldade == 0) {
        return;
    }

    srand((unsigned int)time(NULL));
    inicializarJogo(&jogo, dificuldade);
    posicionarNavios(&jogo);
    mensagem[0] = '\0';
    desistiu = 0;

    while (jogo.tentativas < jogo.maxTentativas && !todosNaviosAfundados(&jogo)) {
        limparTela();
        mostrarCabecalho("B A T A L H A   N A V A L");
        mostrarTabuleiro(&jogo, 0);
        mostrarPainel(&jogo);
        if (mensagem[0] != '\0') {
            printf("\n  %s\n", mensagem);
        }
        printf("\n  Tiro %d - digite linha e coluna (ex.: A 3) ou 0 para desistir: ",
               jogo.tentativas + 1);

        if (!lerLinha(entrada, sizeof(entrada))) {
            printf("\n  Entrada encerrada.\n");
            return;
        }

        status = lerCoordenada(entrada, &linha, &coluna);
        if (status == COORD_DESISTIR) {
            desistiu = 1;
            break;
        }
        if (status == COORD_FORMATO_INVALIDO) {
            snprintf(mensagem, sizeof(mensagem), COR_VERMELHO
                     "Entrada invalida. Use uma letra de A a H e um numero de 1 a 8." COR_RESET);
            continue;
        }
        if (status == COORD_FORA_DO_TABULEIRO) {
            snprintf(mensagem, sizeof(mensagem), COR_VERMELHO
                     "Coordenada fora do tabuleiro. Tente novamente." COR_RESET);
            continue;
        }
        if (jogo.tiros[linha][coluna] != ' ') {
            snprintf(mensagem, sizeof(mensagem), COR_AMARELO
                     "Voce ja atacou %c%d. Escolha outra casa." COR_RESET, 'A' + linha, coluna + 1);
            continue;
        }

        resultado = realizarAtaque(&jogo, linha, coluna);
        if (resultado == RESULTADO_AFUNDOU) {
            snprintf(mensagem, sizeof(mensagem), COR_VERMELHO COR_NEGRITO
                     "%c%d: BOOM! Voce afundou o %s!" COR_RESET,
                     'A' + linha, coluna + 1, jogo.navios[jogo.ultimoNavio].nome);
        } else if (resultado == RESULTADO_ACERTO) {
            snprintf(mensagem, sizeof(mensagem), COR_VERMELHO
                     "%c%d: Acertou o %s!" COR_RESET,
                     'A' + linha, coluna + 1, jogo.navios[jogo.ultimoNavio].nome);
        } else {
            snprintf(mensagem, sizeof(mensagem), COR_AZUL
                     "%c%d: Agua... splash!" COR_RESET, 'A' + linha, coluna + 1);
        }
    }

    limparTela();
    mostrarCabecalho("B A T A L H A   N A V A L");

    if (desistiu) {
        mostrarTabuleiro(&jogo, 1);
        printf("\n  " COR_AMARELO "Voce desistiu da partida. Ela nao foi salva no historico." COR_RESET "\n");
        pausar();
        return;
    }

    venceu = todosNaviosAfundados(&jogo);
    mostrarTabuleiro(&jogo, !venceu);
    mostrarResultado(&jogo, venceu);

    if (!salvarPartida(&jogo, venceu)) {
        printf("\n  " COR_VERMELHO "Aviso: nao foi possivel salvar a partida em %s." COR_RESET "\n",
               ARQUIVO_HISTORICO);
    } else {
        printf("\n  " COR_CINZA "Partida salva no historico." COR_RESET "\n");
    }
    pausar();
}
